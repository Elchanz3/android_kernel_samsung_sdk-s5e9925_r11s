#include "exynos-profiler-fn.h"
#include <soc/samsung/fvmap.h>
#include <linux/module.h>
inline 
u64 * alloc_state_in_freq(int size)
{
	u64 *state_in_state;
	state_in_state = kzalloc(sizeof(u64) * size, GFP_KERNEL);
	if (!state_in_state) {
		pr_err("failed to alloc state_in_freq\n");
		return NULL;
	}
	return state_in_state;
}
EXPORT_SYMBOL(alloc_state_in_freq);
inline
int init_freq_cstate(struct freq_cstate *fc, int size)
{
	int state;

	for_each_cstate(state) {
		fc->time[state] = kcalloc(size, sizeof(u64), GFP_KERNEL);

		if (unlikely(!fc->time[state]))
			goto fail;
	}

	return 0;
fail:
	for_each_cstate(state)
		kfree(fc->time[state]);
	return -ENOMEM;
}
EXPORT_SYMBOL(init_freq_cstate);
inline
int init_freq_cstate_snapshot(struct freq_cstate_snapshot *fc_snap, int size)
{
	int state;

	for_each_cstate(state) {
		fc_snap->time[state] = kcalloc(size, sizeof(u64), GFP_KERNEL);

		if (unlikely(!fc_snap->time[state]))
			goto fail;
	}

	return 0;
fail:
	for_each_cstate(state)
		kfree(fc_snap->time[state]);
	return -ENOMEM;
}
EXPORT_SYMBOL(init_freq_cstate_snapshot);
inline
int init_freq_cstate_result(struct freq_cstate_result *fc_result, int size)
{
	int state;

	for_each_cstate(state) {
		fc_result->time[state] = kcalloc(size, sizeof(u64), GFP_KERNEL);

		if (unlikely(!fc_result->time[state]))
			goto fail;
	}

	return 0;
fail:
	for_each_cstate(state)
		kfree(fc_result->time[state]);
	return -ENOMEM;
}
EXPORT_SYMBOL(init_freq_cstate_result);
inline
void sync_snap_with_cur(u64 *state_in_freq, u64 *snap, int size)
{
	memcpy(snap, state_in_freq, sizeof(u64) * size);
}
EXPORT_SYMBOL(sync_snap_with_cur);
inline
void make_snap_and_delta(u64 *state_in_freq,
			u64 *snap, u64 *delta, int size)
{
	int idx;

	for (idx = 0; idx < size; idx++) {
		u64 delta_time, cur_snap_time;

		if (!state_in_freq[idx])
			continue;

		/* get current time_in_freq */
		cur_snap_time = state_in_freq[idx];

		/* compute delta between currents snap and previous snap */
		delta_time = cur_snap_time - snap[idx];
		snap[idx] = cur_snap_time;
		delta[idx] = delta_time;
	}
}
EXPORT_SYMBOL(make_snap_and_delta);
inline
void sync_fcsnap_with_cur(struct freq_cstate *fc,
		struct freq_cstate_snapshot *fc_snap, int size)
{
	int state;

	for_each_cstate(state) {
		if (!fc->time[state])
			continue;
		memcpy(fc_snap->time[state],
			fc->time[state], sizeof(ktime_t) * size);
	}
	/* update snapshot time */
	fc_snap->last_snap_time = ktime_get();
}
EXPORT_SYMBOL(sync_fcsnap_with_cur);
inline
void make_snapshot_and_time_delta(struct freq_cstate *fc,
			struct freq_cstate_snapshot *fc_snap,
			struct freq_cstate_result *fc_result,
			int size)
{
	int state, idx;

	/* compute time_delta and make snapshot */
	for (state = 0; state < NUM_OF_CSTATE; state++) {
		if (!fc->time[state])
			continue;

		for (idx = 0; idx < size; idx++) {
			u64 delta_time, cur_snap_time;

			/* get current time_in_freq */
			cur_snap_time = fc->time[state][idx];

			/* compute delta between currents snap and previous snap */
			delta_time = cur_snap_time - fc_snap->time[state][idx];
			fc_snap->time[state][idx] = cur_snap_time;
			fc_result->time[state][idx] = delta_time;
		}
	}

	/* save current time */
	fc_result->profile_time = ktime_get() - fc_snap->last_snap_time;
	fc_snap->last_snap_time = ktime_get();
}
EXPORT_SYMBOL(make_snapshot_and_time_delta);
inline
void compute_freq_cstate_result(struct freq_table *freq_table,
	struct freq_cstate_result *fc_result, int size, int cur_freq_idx, int temp)
{
	ktime_t total_time;
	u64 freq, ratio;
	u64 st_power = 0, dyn_power = 0;
	ktime_t profile_time = fc_result->profile_time;
	s32 state, idx;
	s32 temp_weight = temp ? (temp * temp) : 1;
	s32 temp_base_weight = temp ? ST_COST_BASE_TEMP_WEIGHT : 1;

	if (unlikely(!fc_result->profile_time))
		return;

	for_each_cstate(state) {
		if (!fc_result->time[state])
			continue;

		total_time = freq = ratio = 0;
		for (idx = 0; idx < size; idx++) {
			ktime_t time = fc_result->time[state][idx];

			total_time += time;
			freq += time * khz_to_mhz(freq_table[idx].freq);

			if (state == CS_ACTIVE) {
				st_power += (time * freq_table[idx].st_cost) * temp_weight;
				dyn_power += (time * freq_table[idx].dyn_cost);
			}

			if (state == CLK_OFF)
				st_power += (time * freq_table[idx].st_cost) * temp_weight;
		}

		if (likely(total_time)) {
			fc_result->ratio[state] = total_time * RATIO_UNIT / profile_time;
			fc_result->freq[state] = (RATIO_UNIT * freq) / total_time;
		} else {
			fc_result->ratio[state] = 0;
			fc_result->freq[state] = 0;;
		}

		if (!fc_result->freq[state])
			fc_result->freq[state] = freq_table[cur_freq_idx].freq;
	}
	fc_result->dyn_power = dyn_power / profile_time;
	fc_result->st_power = st_power / (profile_time * temp_base_weight);
}
EXPORT_SYMBOL(compute_freq_cstate_result);
#define RELATION_LOW	0
#define RELATION_HIGH	1
/*
 * Input table should be DECENSING-ORDER
 * RELATION_LOW : return idx of freq lower than or same with input freq
 * RELATOIN_HIGH: return idx of freq higher thant or same with input freq
 */
inline u32 get_idx_from_freq(struct freq_table *table,
				u32 size, u32 freq, bool flag)
{
	int idx;

	if (flag == RELATION_HIGH) {
		for (idx = size - 1; idx >=  0; idx--) {
			if (freq <= table[idx].freq)
				return idx;
		}
	} else {
		for (idx = 0; idx < size; idx++) {
			if (freq >= table[idx].freq)
				return idx;
		}
	}
	return flag == RELATION_HIGH ? 0 : size - 1;
}
EXPORT_SYMBOL(get_idx_from_freq);
inline
void compute_power_freq(struct freq_table *freq_table, u32 size, s32 cur_freq_idx,
		u64 *active_in_freq, u64 *clkoff_in_freq, u64 profile_time,
		s32 temp, u64 *dyn_power_ptr, u64 *st_power_ptr, 
		u32 *active_freq_ptr, u32 *active_ratio_ptr)
{
	s32 idx;
	u64 st_power = 0, dyn_power = 0;
	u64 active_freq = 0, total_time = 0;
	s32 temp_weight = temp ? (temp * temp) : 1;
	s32 temp_base_weight = temp ? ST_COST_BASE_TEMP_WEIGHT : 1;

	if (unlikely(!profile_time))
		return;

	for (idx = 0; idx < size; idx++) {
		u64 time;

		if (likely(active_in_freq)) {
			time = active_in_freq[idx];

			dyn_power += (time * freq_table[idx].dyn_cost);
			st_power += (time * freq_table[idx].st_cost) * temp_weight;
			active_freq += time * khz_to_mhz(freq_table[idx].freq);
			total_time += time;
		}

		if (likely(clkoff_in_freq)) {
			time = clkoff_in_freq[idx];
			st_power += (time * freq_table[idx].st_cost) * temp_weight;
		}
	}

	if (active_ratio_ptr)
		*active_ratio_ptr = total_time * RATIO_UNIT / profile_time;

	if (active_freq_ptr) {
		if (likely(active_freq) && likely(total_time))
			*active_freq_ptr = active_freq * RATIO_UNIT / total_time;
		else
			*active_freq_ptr = khz_to_mhz(freq_table[cur_freq_idx].freq);
	}

	if (dyn_power_ptr)
		*dyn_power_ptr = dyn_power / profile_time;

	if (st_power_ptr)
		*st_power_ptr = st_power / (profile_time * temp_base_weight);
}
EXPORT_SYMBOL(compute_power_freq);
inline
unsigned int get_boundary_with_spare(struct freq_table *freq_table,
			s32 max_freq_idx, s32 freq_delta_ratio, s32 cur_idx)
{
	int cur_freq = freq_table[cur_idx].freq;
	int max_freq = freq_table[max_freq_idx].freq;

	return (cur_freq * RATIO_UNIT - max_freq * freq_delta_ratio)
					/ (RATIO_UNIT - freq_delta_ratio);
}
EXPORT_SYMBOL(get_boundary_with_spare);
inline
unsigned int get_boundary_wo_spare(struct freq_table *freq_table,
					s32 freq_delta_ratio, s32 cur_idx)
{
	if (freq_delta_ratio <= 0)
		return freq_table[cur_idx + 1].freq * RATIO_UNIT
				/ (RATIO_UNIT + freq_delta_ratio);

	return freq_table[cur_idx].freq * RATIO_UNIT
				/ (RATIO_UNIT + freq_delta_ratio);
}
EXPORT_SYMBOL(get_boundary_wo_spare);
inline
unsigned int get_boundary_freq(struct freq_table *freq_table,
		s32 max_freq_idx, s32 freq_delta_ratio, s32 flag, s32 cur_idx)
{
	if (freq_delta_ratio <= 0)
		return get_boundary_wo_spare(freq_table, freq_delta_ratio, cur_idx);

	if (flag & STATE_SCALE_WITH_SPARE)
		return get_boundary_with_spare(freq_table, max_freq_idx,
						freq_delta_ratio, cur_idx);

	return get_boundary_wo_spare(freq_table, freq_delta_ratio, cur_idx);

}
EXPORT_SYMBOL(get_boundary_freq);
inline
int get_boundary_freq_delta_ratio(struct freq_table *freq_table,
		s32 min_freq_idx, s32 cur_idx, s32 freq_delta_ratio, u32 boundary_freq)
{
	int delta_pct;
	unsigned int lower_freq;
	unsigned int cur_freq = freq_table[cur_idx].freq;

	lower_freq = (cur_idx == min_freq_idx) ? 0 : freq_table[cur_idx + 1].freq;

	if (freq_delta_ratio > 0)
		delta_pct = (cur_freq - boundary_freq) * RATIO_UNIT
						/ (cur_freq - lower_freq);
	else
		delta_pct = (boundary_freq - lower_freq) * RATIO_UNIT
						/ (cur_freq - lower_freq);
	return delta_pct;
}
EXPORT_SYMBOL(get_boundary_freq_delta_ratio);
inline
unsigned int get_freq_with_spare(struct freq_table *freq_table,
	s32 max_freq_idx, s32 cur_freq, s32 freq_delta_ratio, s32 flag)
{
	if (flag & STATE_SCALE_WITH_ORG_CAP)
		max_freq_idx = 0;

	return cur_freq + ((freq_table[max_freq_idx].freq - cur_freq)
					* freq_delta_ratio / RATIO_UNIT);
}
EXPORT_SYMBOL(get_freq_with_spare);
inline
unsigned int get_freq_wo_spare(struct freq_table *freq_table,
		s32 max_freq_idx, s32 cur_freq, s32 freq_delta_ratio)
{
	return cur_freq + ((cur_freq * freq_delta_ratio) / RATIO_UNIT);
}
EXPORT_SYMBOL(get_freq_wo_spare);
inline
int get_scaled_target_idx(struct freq_table *freq_table,
			s32 min_freq_idx, s32 max_freq_idx,
			s32 freq_delta_ratio, s32 flag, s32 cur_idx)
{
	int idx, cur_freq, target_freq;
	int target_idx;

	cur_freq = freq_table[cur_idx].freq;

	if (flag & STATE_SCALE_WITH_SPARE) {
		if (freq_delta_ratio > 0)
			target_freq = get_freq_with_spare(freq_table, max_freq_idx,
							cur_freq, freq_delta_ratio, flag);
		else
			target_freq = get_freq_wo_spare(freq_table, max_freq_idx,
							cur_freq, freq_delta_ratio);
	} else {
		target_freq = get_freq_wo_spare(freq_table, max_freq_idx,
						cur_freq, freq_delta_ratio);
	}

	if (target_freq == cur_freq)
		target_idx = cur_idx;

	if (target_freq > cur_freq) {
		for (idx = cur_idx; idx >= max_freq_idx; idx--) {
			if (freq_table[idx].freq >= target_freq) {
				target_idx = idx;
				break;
			}
			target_idx = 0;
		}
	} else {
		for (idx = cur_idx; idx <= min_freq_idx; idx++) {
			if (freq_table[idx].freq < target_freq) {
				target_idx = idx;
				break;
			}
			target_idx = min_freq_idx;
		}
	}

	if (abs(target_idx - cur_idx) > 1)
		target_idx = ((target_idx - cur_idx) > 0) ? cur_idx + 1 : cur_idx - 1;

	return target_idx;
}
EXPORT_SYMBOL(get_scaled_target_idx);
inline
void __scale_state_in_freq(struct freq_table *freq_table,
		s32 min_freq_idx, s32 max_freq_idx,
		u64 *active_state, u64 *clkoff_state,
		s32 freq_delta_ratio, s32 flag, s32 cur_idx)
{
	int target_freq, cur_freq, boundary_freq;
	int real_freq_delta_ratio, boundary_freq_delta_ratio, target_idx;
	ktime_t boundary_time;

	target_idx = get_scaled_target_idx(freq_table, min_freq_idx, max_freq_idx,
						freq_delta_ratio, flag, cur_idx);
	if (target_idx == cur_idx)
		return;

	cur_freq = freq_table[cur_idx].freq;
	target_freq = freq_table[target_idx].freq;

	boundary_freq = get_boundary_freq(freq_table, max_freq_idx,
				freq_delta_ratio, flag, cur_idx);
	if (freq_delta_ratio > 0) {
		if (boundary_freq <= freq_table[cur_idx + 1].freq)
			boundary_freq = freq_table[cur_idx + 1].freq;
	} else {
		if (boundary_freq > freq_table[cur_idx].freq)
			boundary_freq = freq_table[cur_idx].freq;
	}

	boundary_freq_delta_ratio = get_boundary_freq_delta_ratio(freq_table,
					min_freq_idx, cur_idx, freq_delta_ratio, boundary_freq);
	real_freq_delta_ratio = (target_freq - cur_freq) / (cur_freq / RATIO_UNIT);

	boundary_time = active_state[cur_idx] * boundary_freq_delta_ratio / RATIO_UNIT;

	if (flag & STATE_SCALE_CNT)
		active_state[target_idx] += boundary_time;
	else
		active_state[target_idx] += (boundary_time * RATIO_UNIT)
					/ (RATIO_UNIT + real_freq_delta_ratio);
	active_state[cur_idx] -= boundary_time;
}
EXPORT_SYMBOL(__scale_state_in_freq);
inline
void scale_state_in_freq(struct freq_table *freq_table,
		s32 min_freq_idx, s32 max_freq_idx,
		u64 *active_state, u64 *clkoff_state,
		s32 freq_delta_ratio, s32 flag)
{
	int idx;

	if (freq_delta_ratio > 0)
		for (idx = max_freq_idx + 1; idx <= min_freq_idx; idx++)
			__scale_state_in_freq(freq_table,
				min_freq_idx, max_freq_idx,
				active_state, clkoff_state,
				freq_delta_ratio, flag, idx);
	else
		for (idx = min_freq_idx - 1; idx >= max_freq_idx; idx--)
			__scale_state_in_freq(freq_table,
				min_freq_idx, max_freq_idx,
				active_state, clkoff_state,
				freq_delta_ratio, flag, idx);
}
EXPORT_SYMBOL(scale_state_in_freq);
inline
void get_power_change(struct freq_table *freq_table, s32 size,
	s32 cur_freq_idx, s32 min_freq_idx, s32 max_freq_idx,
	u64 *active_state, u64 *clkoff_state,
	s32 freq_delta_ratio, u64 profile_time, s32 temp, s32 flag,
	u64 *scaled_dyn_power, u64 *scaled_st_power, u32 *scaled_active_freq)
{
	u64 *scaled_active_state, *scaled_clkoff_state;

	scaled_active_state = kcalloc(size, sizeof(u64), GFP_KERNEL);
	scaled_clkoff_state = kcalloc(size, sizeof(u64), GFP_KERNEL);

	/* copy state-in-freq table */
	memcpy(scaled_active_state, active_state, sizeof(u64) * size);
	memcpy(scaled_clkoff_state, clkoff_state, sizeof(u64) * size);

	/* scaling table with freq_delta_ratio */
	scale_state_in_freq(freq_table, min_freq_idx, max_freq_idx,
			scaled_active_state, scaled_clkoff_state,
			freq_delta_ratio, flag);

	/* computing power with scaled table */
	compute_power_freq(freq_table, size, cur_freq_idx,
		scaled_active_state, scaled_clkoff_state, profile_time, temp,
		scaled_dyn_power, scaled_st_power, scaled_active_freq, NULL);

	kfree(scaled_active_state);
	kfree(scaled_clkoff_state);
}
EXPORT_SYMBOL(get_power_change);
inline u64 pwr_cost(u32 freq, u32 volt, u32 coeff, bool flag)
{
	u64 cost = coeff;
	/*
	 * Equation for power cost
	 * PWR_COST_CFVV : Coeff * F(MHz) * V^2
	 * PWR_COST_CVV  : Coeff * V^2
	 */
	volt = volt >> 10;
	cost = ((cost * volt * volt) >> 20);
	if (flag == PWR_COST_CFVV)
		return (cost * (freq >> 10)) >> 10;

	return cost;
}
EXPORT_SYMBOL(pwr_cost);
inline
int init_static_cost(struct freq_table *freq_table, int size, int weight,
			struct device_node *np, struct thermal_zone_device *tz)
{
	int volt_size, temp_size;
	int *table = NULL;
	int freq_idx, idx, ret = 0;

	ret = exynos_build_static_power_table(np, &table, &volt_size, &temp_size, tz->type);
	if (ret) {
		int cal_id, ratio, asv_group, static_coeff;
		int ratio_table[9] = { 5, 6, 8, 11, 15, 20, 27, 36, 48 };

		pr_info("%s: there is no power table for %s\n", __func__, tz->type);

		// Make power table from coeff and ids
		if (of_property_read_u32(np, "cal-id", &cal_id)) {
			if (of_property_read_u32(np, "g3d_cmu_cal_id", &cal_id)) {
				pr_err("%s: Failed to get cal-id\n", __func__);
				return -EINVAL;
			}
		}

		if (of_property_read_u32(np, "static-power-coefficient", &static_coeff)) {
			pr_err("%s: Failed to get staic_coeff\n", __func__);
			return -EINVAL;
		}

		ratio = cal_asv_get_ids_info(cal_id);
		asv_group = cal_asv_get_grp(cal_id);


		if (asv_group < 0 || asv_group > 8)
			asv_group = 5;

		if (!ratio)
			ratio = ratio_table[asv_group];

		static_coeff *= ratio;

		pr_info("%s power table through ids (weight=%d)\n", tz->type, weight);
		for (idx = 0; idx < size; idx++) {
			freq_table[idx].st_cost = pwr_cost(freq_table[idx].freq,
					freq_table[idx].volt, static_coeff, PWR_COST_CVV) / weight;
			pr_info("freq_idx=%2d freq=%d, volt=%d cost=%lld\n",
					idx, freq_table[idx].freq, freq_table[idx].volt, freq_table[idx].st_cost);
		}

		return 0;
	}

	pr_info("%s power table from DTM (weight=%d)\n", tz->type, weight);
	for (freq_idx = 0; freq_idx < size; freq_idx++) {
		int freq_table_volt = freq_table[freq_idx].volt / 1000;

		for (idx = 1; idx <= volt_size; idx++) {
			int cost;
			int dtm_volt = *(table + ((temp_size + 1) * idx));

			if (dtm_volt < freq_table_volt)
				continue;

			cost = *(table + ((temp_size + 1) * idx + 1));
			cost = cost / weight;
			freq_table[freq_idx].st_cost = cost;
			pr_info("freq_idx=%2d freq=%d, volt=%d dtm_volt=%d, cost=%8d\n",
					freq_idx, freq_table[freq_idx].freq, freq_table_volt,
					dtm_volt, cost);
			break;
		}
	}

	kfree(table);

	return 0;
}
EXPORT_SYMBOL(init_static_cost);
inline int is_vaild_freq(u32 *freq_table, u32 table_cnt, u32 freq)
{
	int idx;
    for (idx = 0; idx < table_cnt; idx++)
		if(freq_table[idx] == freq)
			return true;
    return false;
}
EXPORT_SYMBOL(is_vaild_freq);
inline struct freq_table *init_freq_table (u32 *freq_table, u32 table_cnt,
                    u32 cal_id, u32 max_freq, u32 min_freq,
                    u32 dyn_ceff, u32 st_ceff,
                    bool dyn_flag, bool st_flag)
{
    struct freq_table *table;
    unsigned int *cal_vtable, cal_table_cnt;
    unsigned long *cal_ftable;
    int cal_idx, idx = 0;

    /* alloc table */
    table = kzalloc(sizeof(struct freq_table) * table_cnt, GFP_KERNEL);
    if (!table) {
        pr_err("profiler: failed to alloc table(min=%d max=%d cnt=%d)\n",
                    min_freq, max_freq, table_cnt);
        return NULL;
    }
    /* get cal table including invalid and valid freq */
    cal_table_cnt = cal_dfs_get_lv_num(cal_id);
    cal_ftable = kzalloc(sizeof(unsigned long) * cal_table_cnt, GFP_KERNEL);
    cal_vtable = kzalloc(sizeof(unsigned int) * cal_table_cnt, GFP_KERNEL);
    if (!cal_ftable || !cal_vtable) {
        pr_err("profiler: failed to alloc cal-table(cal-cnt=%d)\n",
                                cal_table_cnt);
        kfree(table);

        if (cal_ftable)
            kfree(cal_ftable);

        if (cal_vtable)
            kfree(cal_vtable);

        return NULL;
    }
    cal_dfs_get_rate_table(cal_id, cal_ftable);
    cal_dfs_get_asv_table(cal_id, cal_vtable);
	/* fill freq/volt and dynamic/static power cost */
	for (cal_idx = 0; cal_idx < cal_table_cnt; cal_idx++) {
		unsigned int freq = cal_ftable[cal_idx];
		//if (freq > max_freq)
		//  continue;
		if (freq < min_freq)
			continue;

		if (freq_table && !is_vaild_freq(freq_table, table_cnt, freq))
			continue;

		if (idx >= table_cnt) {
			pr_err("profiler: table_cnt is wrong(tcnt=%d freq=%d minf=%d maxf=%d\n",
							table_cnt, freq, min_freq, max_freq);
			break;
		}

		table[idx].freq = freq;
		table[idx].volt = cal_vtable[cal_idx];
		table[idx].dyn_cost = pwr_cost(table[idx].freq,
					table[idx].volt, dyn_ceff, dyn_flag);
		table[idx].st_cost = pwr_cost(table[idx].freq,
					table[idx].volt, st_ceff, st_flag);
		idx++;
	}
	kfree(cal_ftable);
	kfree(cal_vtable);

	return table;
}
EXPORT_SYMBOL(init_freq_table);

inline struct freq_table *init_fvtable (u32 *freq_table, u32 table_cnt,
                    u32 cal_id, u32 max_freq, u32 min_freq,
                    u32 dyn_ceff, u32 st_ceff,
                    bool dyn_flag, bool st_flag)
{
    struct freq_table *table;
	struct freq_volt *fvtable;
	//int freq_table_size = table_cnt;
	int i,j;

    /* alloc table */
    table = kzalloc(sizeof(struct freq_table) * table_cnt, GFP_KERNEL);
    if (!table) {
        pr_err("profiler: failed to alloc table(min=%d max=%d cnt=%d)\n",
                    min_freq, max_freq, table_cnt);
        return NULL;
    }
	fvtable = kcalloc(table_cnt, sizeof(struct freq_volt),
				GFP_KERNEL);
	if (!fvtable) {
		pr_err("profiler: failed to alloc fvtable(min=%d max=%d cnt=%d)\n",
                    min_freq, max_freq, table_cnt);
		kfree(table);
		return NULL;
	}
	for (i = 0, j = 0; i < table_cnt; i++) {
		if (freq_table[table_cnt - 1 - i] > max_freq ||
			freq_table[table_cnt - 1 - i] < min_freq)
			continue;

		fvtable[j++].rate = freq_table[table_cnt - 1 - i];
	}
	cal_dfs_get_freq_volt_table(cal_id, fvtable, table_cnt);

	/* fill freq/volt and dynamic/static power cost */
	for (i = 0, j = table_cnt -1 ; i < table_cnt; i++) {
		unsigned int freq = fvtable[i].rate;
		unsigned int volt = fvtable[i].volt;
		//if (freq > max_freq)
		//  continue;
		if (freq < min_freq)
			continue;

		if (i >= table_cnt) {
			pr_err("profiler: table_cnt is wrong(tcnt=%d freq=%d minf=%d maxf=%d\n",
							table_cnt, freq, min_freq, max_freq);
			break;
		}

		table[j].freq = freq;
		table[j].volt = volt;
		table[j].dyn_cost = pwr_cost(table[j].freq,
					table[j].volt, dyn_ceff, dyn_flag);
		table[j].st_cost = pwr_cost(table[j].freq,
					table[j].volt, st_ceff, st_flag);
		j--;
	}
	kfree(fvtable);
	return table;
}
EXPORT_SYMBOL(init_fvtable);
inline struct thermal_zone_device * init_temp (const char *name)
{
    return thermal_zone_get_zone_by_name(name);
}
EXPORT_SYMBOL(init_temp);
inline s32 get_temp(struct thermal_zone_device *tz)
{
    s32 temp;
    thermal_zone_get_temp(tz, &temp);
    return temp / 1000;
}
EXPORT_SYMBOL(get_temp);


MODULE_LICENSE("GPL");
