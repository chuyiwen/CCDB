/*******************************************************************************

Copyright 2010 by tiankong Interactive Game Co., Ltd.
All rights reserved.

This software is the confidential and proprietary information of
tiankong Interactive Game Co., Ltd. ('Confidential Information'). You shall
not disclose such Confidential Information and shall use it only in
accordance with the terms of the license agreement you entered into with
tiankong Interactive  Co., Ltd.

*******************************************************************************/

#ifndef __DATABASE_HANDLRR_H__

#define __DATABASE_HANDLRR_H__

// 人物
struct s_role_data_load;
struct s_role_data_save;
struct s_create_role_info;
struct s_skill_save;
struct s_enter_map_limit;
struct NET_DB2C_change_role_money;
struct NET_DB2C_update_role_send_num;

// 道具、装备
struct tagItem;
struct tagEquip;
struct s_equip_spec_update;
struct tagEquipSpec;
struct NET_DB2S_load_equip_rank;
// 宠物
struct s_db_pet_soul_create;
struct s_db_pet_skill;
struct s_db_pet_att;
struct tagPetSNSInfo;
// 团购
struct s_gp_info_key;
struct s_gp_info;
// 摆摊
struct s_vip_stall_to_db;
//VIP网吧
struct s_db_vnb_players;
// 帮会
struct s_create_guild_info;
struct s_create_guild_member_info;
struct tagGuildBase;
struct s_facilities_load;
struct s_guild_skill_info;
struct NET_DB2C_change_guild_pos_name;
struct NET_DB2C_change_guild_pos_power;
struct NET_DB2C_change_guild_symbol;
struct NET_DB2C_insert_pvp_data;
struct NET_DB2C_up_pvp_data;
struct s_guild_kill_boss;
struct tagGuildWarHistory;
// 邮件
struct NET_DB2C_save_mail;
struct NET_DB2C_update_mail;
struct NET_DB2C_delete_mail;
// 拍卖
struct NET_S2DB_insert_paimai;
struct NET_S2DB_delete_paimai;
struct NET_S2DB_update_paimai;
struct NET_S2DB_update_auto_paimai;

// 钱庄
struct NET_S2DB_insert_bank;
struct NET_S2DB_delete_bank;
struct NET_S2DB_update_bank;

struct s_guild_plant;
//--------------------------------------------------------------------
// 数据库处理类，执行查询、更新、删除、替换(replace into)操作
// 提供一个通用的方法: 有结果集返回: common_select (查询)
//					   无结果集返回: common_update (更新、删除、替换)
//!!!!!				   无结果集异步日志操作: common_update_log(更新、删除、替换)					  
// 1.所需要的内存全由外部调用者分配并管理，此类只使用不管理
// 2.获得数据库返回的结果集，可构造不定长的网络数据包
// 3.向数据库写入或者更新多条数据时，每条数据的大小必须一致
// 4.调用common_update只需提供一个格式化相应字段形如func_param3_1的函数 
// 5.调用common_select出了第四条所需函数还需要提供一个处理结果集形如func_result_proc的函数
// 6.日志实时性不高,都采用异步方式需要形如func_param3_2的函数
//--------------------------------------------------------------------

// 数据库处理函数
typedef fastdelegate::FastDelegate3<odbc::sql_language_disposal*, DWORD, DWORD, VOID> func_param3;
typedef fastdelegate::FastDelegate3<odbc::sql_language_disposal*, DWORD, LPVOID, VOID> func_param3_1;
typedef fastdelegate::FastDelegate3<odbc::sql_language_disposal*, LPCSTR, LPVOID, VOID> func_param3_2;
typedef fastdelegate::FastDelegate3<LPVOID&, int*, odbc::execute_result*, VOID> func_result_proc;
#define  MakeDelegateOfDBHandler(func) fastdelegate::MakeDelegate(this,func)

class db_server;
class db_handler
{
public:
	db_handler();
	~db_handler();

public:
	// 启动函数
	BOOL start(db_server* p_db_server, LPCTSTR sz_init_file, few_connect_server* p_server);
	
	// 销毁函数
	VOID destroy();
public: // 账号系统
	// 账号公用
	BOOL load_one_account_common(LPVOID p_buffer, DWORD dw_account_id);

	// 添加公用
	BOOL insert_account_common(DWORD dw_account_id, const char* sz_account);

	// 更新安全码
	BOOL update_safe_code(DWORD dw_account_id, DWORD dw_safe_code_crc);

	// 设置安全码重置时间
	BOOL update_safe_code_reset_time(DWORD dw_account_id, DWORD dw_reset_time);

	// 更新背包密码
	BOOL update_bag_password(DWORD dw_account_id, DWORD dw_bag_password);

	// 跟新角色仓库上限
	BOOL update_ware_size(DWORD dw_account_id, LPVOID p_buffer);

	// 更新仓库货币
	BOOL update_ware_money(DWORD dw_account_id, int64 n64_ware_silver);

	// 更新包裹非绑定币
	BOOL update_bag_money(DWORD	dw_role_id,	LPVOID p_buffer);

	// 更新元宝
	BOOL update_baibao_yuanbao(DWORD dw_account_id, LPVOID p_buffer);

	// 更新积分
	BOOL update_exchang_volume(DWORD dw_account_id, LPVOID p_buffer);

	// 更新领取标志
	BOOL update_receive(DWORD dw_account_id, LPVOID p_buffer);

	// 更新领取标志
	BOOL update_receive_ex(DWORD dw_account_id, LPVOID p_buffer);

	// 更新网页领奖标志
	BOOL update_web_receive(DWORD dw_account_id, LPVOID p_buffer);

	// 更新充值总量
	BOOL update_total_recharge(DWORD dw_account_id, LPVOID p_buffer);

	// 删除序列号礼包
	BOOL delete_serial_reward(DWORD dw_account_id, LPVOID p_buffer);

	// 锁表
	BOOL lock_table_account_common();
private:
	VOID makesql_load_one_account_common(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_insert_account_common(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_update_safe_code(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_update_safe_code_reset_time(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_update_bag_password(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_update_ware_size(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_update_ware_money(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_update_bag_money(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_baibao_yuanbao(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_update_exchange_volume(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_update_receive(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_update_receive_ex(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_update_total_recharge(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_update_web_receive(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_delete_serial_reward(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
private:
	VOID procres_load_one_account_common(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// 元宝账户
	// 读取元宝账户
	BOOL load_all_yuanbao_account(LPVOID p_buffer, int &n_count);

	// 读取元宝订单
	BOOL load_all_yuanbao_order(LPVOID p_buffer, int &n_count);

	// 读取角色元宝订单
	BOOL load_one_role_all_yuanbao_order(LPVOID p_buffer, int&n_count, DWORD dw_role_id);

	// 新增元宝账户
	BOOL insert_yuanbao_account(DWORD dw_role_id);

	// 更新账户元宝
	BOOL update_account_yuanbao(DWORD dw_role_id, int n_yuanbao);

	// 更新账户金钱
	BOOL update_account_silver(DWORD dw_role_id, LPVOID p_money);

	// 订单出售
	BOOL update_submit_sell(DWORD dw_role_id, bool b_sell);

	// 订单收购
	BOOL update_submit_buy(DWORD dw_role_id, bool b_buy);

	// 新增订单
	BOOL insert_one_order(DWORD dw_role_id, LPVOID p_order);

	// 完成订单
	BOOL complete_one_order(DWORD dw_role_id, LPVOID p_order);

	// 刷新订单
	BOOL refresh_one_order(DWORD dw_role_id, LPVOID p_order);

	// 删除订单
	BOOL delete_one_order(DWORD dw_role_id, LPVOID p_del_time);
private:
	VOID makesql_load_all_yuanbao_account(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_all_yuanbao_order(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_all_yuanbao_order(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_insert_yuanbao_account(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_account_yuanbao(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_account_silver(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_money);
	VOID makesql_update_submit_sell(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_submit_buy(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_insert_one_order(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_complete_one_order(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_refresh_one_order(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_one_order(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID procres_load_all_yuanbao_account(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_all_yuanbao_order(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_all_yuanbao_order(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:
	// 创建角色
	BOOL create_role(OUT DWORD &dw_new_role_id, DWORD dw_account_id, const s_create_role_info &role_info, DWORD &dw_role_name_crc);

	// 备份角色
	DWORD backup_role(DWORD dw_account_id, DWORD dw_role_id);

	// 删除角色
	DWORD delete_role(DWORD dw_account_id, DWORD dw_role_id);

	// 记录删除角色
	BOOL register_deleted_role(DWORD dw_account_id, DWORD dw_role_id, LPCTSTR sz_role_name, LPCSTR sz_ip);

	// 激活删除角色
	BOOL active_deleted_role(DWORD dw_role_id);

	// 读取所有玩家基本数据
	BOOL load_all_role_simple(LPVOID p_buffer, int &n_role_count);

	// 读取角色简单数据
	BOOL load_one_role_simple(DWORD dw_account_id, int &n_role_count, LPVOID p_sample_role);

	// 读取角色完整数据
	BOOL load_one_role_full_data(DWORD dw_account_id, DWORD dw_role_id, s_role_data_load *p_role_data);

	// 存储角色
	VOID save_one_role(DWORD dw_role_id, s_role_data_save *p_role_data);


	// 读取序列号礼包
	BOOL load_serial_reward(LPVOID p_buffer, DWORD dw_account_id);
private:
	// 备份角色
	BOOL do_backup_role(DWORD dw_account_id, DWORD dw_role_id);
	// 备份道具
	BOOL do_backup_item(DWORD dw_role_id);
	// 删除角色
	BOOL do_delete_role(DWORD dw_account_id, DWORD dw_role_id);

private://读取玩家资料
	// 获取角色基础数据
	BOOL load_one_role_data(LPVOID &p_buffer, DWORD dw_account_id, DWORD dw_role_id);
	
	// 获取角色技能
	BOOL load_one_role_skill(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取角色buff
	BOOL load_one_role_buff(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取角色称号
	BOOL load_one_role_title(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取角色道具
	BOOL load_one_role_items(LPVOID &p_buffer, int &n_count, DWORD dw_account_id, DWORD dw_role_id);

	// 获取角色好友
	BOOL load_one_role_friends(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取角色仇人
	BOOL load_one_role_enemies(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取角色未完成任务
	BOOL load_one_role_quest(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取角色完成任务
	BOOL load_one_role_quest_done(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取完成任务基本数据
	BOOL load_one_role_simple_quest_done(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取道具CD时间
	BOOL load_one_role_item_cd(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取角色友好度
	BOOL load_one_role_friendship(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取我的友好度
	BOOL load_one_role_friendship_ex(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取黑名单
	BOOL load_one_role_black(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取进图限制
	BOOL load_one_role_map_limit(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取完成成就
	BOOL load_complate_achievement(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取成就进度
	BOOL load_progress_achievement(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取副本进度
	BOOL load_process_instance(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// 获取签到数据
	BOOL load_sign_data(LPVOID &p_buffer, DWORD dw_role_id);
	
	// 获取魂晶数据
	BOOL load_huenjing_data(LPVOID &p_buffer, DWORD dw_role_id);
	
	// 获取奖励数据
	BOOL load_reward_data(LPVOID &p_buffer, DWORD dw_role_id);
	
	// 坐骑数据
	BOOL load_mounts_data(LPVOID &p_buffer, DWORD dw_role_id);
private:
	VOID makesql_load_all_role_simple(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_simple(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_skill(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_buff(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_title(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_complate_achievement(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_progress_achievement(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_items(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_friends(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_enemies(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_task(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_quest_done(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_simple_quest_done(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_item_cd(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_friendship(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_friendship_ex(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_black(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_one_role_inst_process(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_role_sign_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_role_huenjing_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_role_reward_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_role_mounts_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
private:
	VOID procres_load_all_role_simple(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_simple(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_skill(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_map_limit(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_buff(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_title(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_complate_achievement(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_progress_achievement(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_items(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_friends(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_enemies(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_quest(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_quest_done(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_simple_quest_done(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_item_cd(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_frienship(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_black(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_one_role_inst_process(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_role_sign_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_role_huenjing_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_role_reward_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_role_mounts_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
public:// 修改人物属性
	// 角色改名
	BOOL change_role_name(DWORD dw_account_id, DWORD dw_role_id, LPCTSTR sz_new_role_name, DWORD dw_name_crc, DWORD dw_time);

	// 恢复删除角色保护时间
	BOOL role_guard_cancel_time(DWORD dw_account_id, DWORD dw_role_id);

	// 设置删除角色保护时间
	BOOL role_guard_set_time(DWORD dw_account_id, DWORD dw_role_id, DWORD dw_time);

	// 角色更改帮会
	BOOL change_role_guild(DWORD dw_role_id, DWORD dw_guild_id);

	// 角色离开帮会时间
	BOOL update_role_leave_guild_time(DWORD dw_role_id, LPVOID p_buffer);

	// 更新角色VIp信息
	BOOL update_role_vip_info(DWORD dw_role_id,LPVOID p_buffer);

	// 设置角色删除标志
	BOOL set_role_delete_flag(DWORD dw_role_id, DWORD dw_time);

	// 角色穿着模型ID
	BOOL change_role_dress_model_id(DWORD dw_role_id, WORD w_value);

	// 更新进入限制
	BOOL update_role_map_limit(DWORD dw_role_id, int n_type);

	// 删除进入限制
	BOOL delete_role_map_limit(DWORD dw_role_id);

	// 新增进入限制
	BOOL insert_role_map_limit(DWORD dw_role_id, s_enter_map_limit* p_map_limit);

	// 更新角色属性
	BOOL update_role_attribute(DWORD dw_role_id, PVOID p_buffer);

	// 更新角色属性点
	BOOL update_role_attribute_point(DWORD dw_role_id, LPVOID p_msg);

	// 更新角色天赋点
	BOOL update_role_talent_point(DWORD dw_role_id, LPVOID p_msg);

	// 清空角色不在线经验
	BOOL clear_role_leave_exp(DWORD dw_role_id, LPVOID p_msg);

	// 清空角色不在线义气值
	BOOL clear_role_leave_brotherhood(DWORD dw_role_id, LPVOID p_msg);

	// 清空配偶信息
	BOOL clear_role_spouse_id(DWORD dw_role_id, LPVOID p_msg);
	// 清空VIP信息
	BOOL clear_role_vip_info(DWORD dw_role_id,LPVOID p_msg);

	// 清空角色挂机次数
	BOOL clear_role_hang_count(DWORD dw_role_id, LPVOID p_msg);
	
	BOOL clear_role_day_clear(DWORD dw_role_id, LPVOID p_msg);
	BOOL reset_role_at_0(DWORD dw_role_id, LPVOID p_msg);
	// 清空角色签到数据
	BOOL clean_role_sign(DWORD dw_role_id, LPVOID p_msg);

	// 清空角色元宝兑换次数
	BOOL clear_role_yuanbao_exchange_num(DWORD dw_role_id, LPVOID p_msg);
	
	// 清空角色拍卖次数
	BOOL clear_role_paimai_count(DWORD dw_role_id, LPVOID p_msg);

	// 清空角色钱庄拍卖次数
	BOOL clear_role_bank_count(DWORD dw_role_id, LPVOID p_msg);

	// 清空角色战功商店购买次数
	BOOL clean_role_exploits_num(DWORD dw_role_id, LPVOID p_msg);

	// 清空角色活跃度数据
	BOOL clean_role_active_data(DWORD dw_role_id, LPVOID p_msg);

	// 更新1v1每日积分
	BOOL update_day_1v1_score(DWORD dw_role_id, LPVOID p_msg);

	// 更新1v1周积分
	BOOL update_week_1v1_score(DWORD dw_role_id, LPVOID p_msg);
	BOOL reset_role_1v1_score(DWORD dw_role_id, LPVOID p_msg);

	// 更新1v1领奖标志
	BOOL update_1v1_award(DWORD dw_role_id, LPVOID p_msg);

	// 删除副本进度
	BOOL delete_inst_process_type(INT16 n_type);
private:
	VOID makesql_update_change_role_guild(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_role_leave_guild_time(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_set_role_delete_flag(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID pTime);
	VOID makesql_change_role_dress_model_id(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_role_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_role_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_role_attribute(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_update_role_attribute_point(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_update_role_talent_point(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_clear_role_leave_exp(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_clear_role_leave_brotherhood(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_clear_role_spouse_id(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);//gx add
	VOID makesql_clear_role_vip_info(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);//gx add
	VOID makesql_update_role_vip_info(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);//gx add
	VOID makesql_clear_role_hang_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_clear_role_day_clear(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_clear_role_exchange_num(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_clear_role_paimai_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_clear_role_bank_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_clear_role_exploits_num(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_clear_role_active_data(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_clean_role_sign(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_update_day_1v1_score(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_update_week_1v1_score(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_reset_role_1v1_score(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_update_role_1v1_award(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
	VOID makesql_reset_role_at_0(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, PVOID p_buffer);
private:
	// 保存角色数据
	BOOL save_role_data(DWORD dw_role_id, const s_role_data_save *p_role_data);

	// 保存家族信息
	BOOL save_role_clan_data(DWORD dw_role_id, IN LPVOID p_buffer, BOOL bChange, OUT LPVOID* pp_buffer);

	// 保存角色技能
	BOOL save_role_skill(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// 保存角色BUFF
	BOOL save_role_buff(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// 保存角色称号
	BOOL save_role_title(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// 保存进入地图限制
	BOOL save_enter_map_limit(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// 完成成就
	BOOL save_complate_achievement(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// 成就进度
	BOOL save_process_achievement(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// 副本进度
	BOOL save_process_instance(DWORD	dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);
	// 删除副本进度
	BOOL delete_inst_process(DWORD	dw_role_id);

	// 签到数据
	BOOL save_sign_data(DWORD	dw_role_id, LPVOID p_buffer, OUT LPVOID *pp_buffer);
	
	BOOL save_huenjing_data(DWORD	dw_role_id, LPVOID p_buffer, OUT LPVOID *pp_buffer);

public:	BOOL save_reward_data(DWORD dw_role_id, LPVOID p_buffer, OUT LPVOID *pp_buffer);//声明改为public gx modify 2013.9.17

private:	
	VOID makesql_save_role_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_save_item_attribute(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_save_role_skill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_save_role_buff(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_save_role_title(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_save_role_task(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_task);
	VOID makesql_save_role_task_done(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_task);
	VOID makesql_delete_role_buff(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_delete_all_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_insert_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_save_map_limit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_save_complate_achievement(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_save_process_achievement(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_save_process_instance(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_delete_process_instance(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_delete_process_instance_type(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_save_sign_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_save_huenjing_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_save_reward_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
private:
	// 删除buff
	BOOL delete_role_buffs(DWORD dw_role_id);
	// 删除物品
	BOOL delete_role_items(DWORD dw_role_id);

public:// 物品装备表操作

	// 新增物品
	BOOL insert_item(const tagItem &item, const char* sz_table = "item");

	// 新增装备
	BOOL insert_equip(const tagEquip &equip, BOOL b_baibao = FALSE);

	// 新增装备属性
	BOOL insert_equip_special(const tagEquip &equip, const char *sz_table = "equip");

	// 保存物品
	BOOL save_item(const tagItem &item);

	// 保存装备属性
	BOOL save_item_attribute(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// 保存装备
	BOOL save_equip(const tagEquip &equip);

	// 保存装备属性
	BOOL save_equip_special(const tagEquip &equip);
	BOOL save_equip_special(const s_equip_spec_update &equip_special_update);

	// 删除物品
	BOOL delete_item(int64 n64_serial, const char *sz_table = "item");

	// 删除装备
	BOOL delete_equip(int64 n64_serial);

	// 删除装备属性
	BOOL delete_equip_special(int64 n64_serial, char *sz_table = "equip");
private:
	VOID makesql_insert_item(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_save_item(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID pItem);
	VOID makesql_insert_equip_special(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_save_equip_special(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_equip);
	VOID makesql_update_equip_special(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_equip_special_update);
	VOID makesql_update_equip_special_common(odbc::sql_language_disposal *p_stream, const tagEquipSpec *p_equip_special);
	VOID makesql_delete_item(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_equip_special(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);

public: //备份删除的道具和装备
	BOOL backup_deleted_equip(int64 n64_serial);
	BOOL backup_deleted_item(int64 n64_serial);

public:// 更新道具CD时间
	BOOL replace_item_cd_time(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);


public:// 角色技能
	
	// 新增技能
	BOOL insert_skill(DWORD dw_role_id, s_skill_save *p_buffer);

	// 删除技能
	BOOL delete_skill(DWORD dw_role_id, DWORD dw_skill_id);

	//更新技能
	BOOL update_skill(DWORD dw_role_id, s_skill_save *p_buffer);
private:
	VOID makesql_insert_skill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_skill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);

public:// 好友

	// 新加好友
	BOOL insert_friend(DWORD dw_role_id, LPVOID p_friend_save);

	// 更新好友
	BOOL update_friend(DWORD dw_role_id, LPVOID p_friend_save);

	// 删除好友
	BOOL delelte_friend(DWORD dw_role_id, DWORD dw_friend_id);

	// 删除角色在好友表中所有记录
	BOOL delete_all_friend_relation(DWORD dw_role_id);
private: 
	VOID makesql_insert_friend(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_save);
	VOID makesql_update_friend(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_save);
	VOID makesqll_delete_friend(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_id);
	VOID makesql_delete_all_friend_relation(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);

public:// 友好度
	
	// 新增好友度
	BOOL insert_friendship(DWORD dw_role_id, LPVOID p_friendship_save);

	// 更新好友度
	BOOL update_friendship(DWORD dw_role_id, LPVOID p_friendship_save);

	// 删除好友度
	BOOL delete_friendship(DWORD dw_role_id, DWORD dw_friend_id);

	// 删除角色时，删除所有好友度相关
	BOOL delete_all_friendship_relation(DWORD dw_role_id);
private:
	VOID makesql_insert_friendship(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friendship_save);
	VOID makesql_update_friendship(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friendship_save);
	VOID makesql_delete_friendship(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_id);
	VOID makesql_delete_all_friendship_relation(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);

public:// 黑名单
	
	// 新加黑名单
	BOOL insert_black(DWORD dw_role_id, DWORD dw_black_id);

	// 删除黑名单
	BOOL delete_black(DWORD dw_role_id, DWORD dw_black_id);

	// 所有和某人有关的黑名单信息，删号时候调用
	BOOL delete_black_all(DWORD dw_role_id);
private:
	VOID makesql_insert_black(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_black_id);
	VOID makesql_delete_black(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_black_id);
	VOID makesql_delete_black_all(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_black_id);
public:// 敌人

	// 新加敌人
	BOOL insert_enemy(DWORD dw_role_id, DWORD dw_enemy_id);
	// 删除敌人
	BOOL delete_enemy(DWORD dw_role_id, DWORD dw_enemy_id);
	// 所有和某人有关的仇人信息，删号时候调用
	BOOL delete_enemy_all(DWORD dw_role_id);
private:
	VOID makesql_insert_enemy(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_enemy_id);
	VOID makesql_delete_enemy(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_enemy_id);
	VOID makesql_delete_enemy_all(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_enemy_id);
public:// 称号

	// 新增称号
	BOOL insert_title(DWORD dw_role_id, LPVOID p_buffer);
	// 删除称号
	BOOL delete_title(DWORD dw_role_id, DWORD dw_title_id);
private:
	VOID makesql_insert_title(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_title(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_title_id);
public:	// 任务
	
	// 新增任务
	BOOL insert_quest(DWORD dw_role_id, LPVOID p_accept_quest);

	// 保存任务
	BOOL save_quest(DWORD dw_role_id, LPVOID p_task, int n_count, OUT LPVOID *pp_buffer);

	// 保存完成任务
	BOOL save_quest_done(DWORD dw_role_id, LPVOID p_task, int n_count);

	// 删除任务
	BOOL delete_quest(DWORD dw_role_id, DWORD dw_task_id);
	BOOL delte_all_dayquest(DWORD nNewFlag);//gx add
	BOOL delte_all_dayquest_done(DWORD nNewFlag);
	BOOL delete_all_teamshare_quest(DWORD nNewFlag);
private:
	VOID makesql_insert_quest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_accept_quest);
	VOID makesql_delete_quest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_task_id);
	VOID makesql_delete_all_teamshare_quest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_NewFlag);
	VOID makesql_delete_all_day_quest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_NewFlag);	
	VOID makesql_delete_all_day_quest_done(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_NewFlag);
public:
	// 获取百宝物品
	BOOL load_item_baibao(DWORD dw_account_id, int n_load, int64 n64_serial, int &n_loaded, int &n_data_size, LPVOID p_buffer);
	// 获取百宝元宝
	BOOL load_baibao_yuanbao(DWORD dw_account_id, int& n_yuanbao);
	// 获取百宝日志
	BOOL load_baibao_log_info(DWORD dw_account_id, int &n_loaded, int &n_data_size, LPVOID p_buffer);
	// 获取需log的item类型
	BOOL load_item_need_log(LPVOID pNeedLogItem, int &n_item_count);
	// 新增百宝日志
	BOOL insert_baibao_log_info(DWORD dw_account_id, LPVOID p_buffer);
	// 更新元宝充值
	BOOL update_yuanbao_recharge(DWORD dw_account_id, INT32 n32_yuanbao);
	// 获取网页领奖标志
	BOOL load_web_receive(DWORD dw_account_id, DWORD& dw_web_receive);
private:
	VOID makesql_load_item_baibao(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_baibao_yuanbao(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_baibao_log_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_insert_baibao_log_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_delete_baibao_log_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_load_item_need_log(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);	
	VOID makesql_update_yuanbao_recharge(odbc::sql_language_disposal* p_stream, DWORD dw_account_id, LPVOID p_buffer);
	VOID makesql_load_web_receive(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
private:
	VOID procres_load_baibao_log_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_count_and_delete_baibao_log(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_baibao_yuanbao(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_item_need_log(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_web_receive(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public: // 邮件系统

	// 获取邮件物品
	BOOL load_mail_item(LPVOID p_buffer);

	// 保存邮件物品
	BOOL update_mail_item(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// 更新邮件
	BOOL update_mail(NET_DB2C_update_mail* p_update_mail);
	
	// 获取所有邮件
	BOOL load_all_mail(LPVOID p_buffer);

	// 获取邮件内容
	BOOL load_mail_content(LPVOID p_buffer);

	// 保存邮件
	BOOL save_mail(NET_DB2C_save_mail* p_save_mail);

	//  删除邮件
	BOOL delete_mail(NET_DB2C_delete_mail* p_delete_mail);

	// 获取邮件最大编号
	BOOL get_mail_max_serial(DWORD& dw_mail_serial);

	// 更改任务金钱
	BOOL change_role_money(NET_DB2C_change_role_money* p_chang_role_money);

	// 更新角色发邮件计数
	BOOL update_role_send_mail_count(NET_DB2C_update_role_send_num* p_update_send_num);

private:
	VOID makesql_load_mail_item(odbc::sql_language_disposal* p_stream, DWORD dw_mail_serial, DWORD dw_role_id);
	VOID makesql_load_all_mail(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_mail_content(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dwMailID);
	VOID makesql_save_mail(odbc::sql_language_disposal *p_stream, DWORD dw_mail_id, LPVOID p_buffer);
	VOID makesql_delete_mail(odbc::sql_language_disposal* p_stream, DWORD dw_mail_id, LPVOID p_buffer);	
	VOID makesql_update_mail(odbc::sql_language_disposal* p_stream, DWORD dw_mail_id, LPVOID p_buffer);
	VOID makesql_change_role_money(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_role_send_mail_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_clear_role_send_mail_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID procres_laod_all_mail(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_mail_content(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_mail_item(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:
	//  获取拍卖最大编号
	BOOL get_paimai_max_id(DWORD& dw_paimai_id);
	// 更新拍卖物品
	BOOL update_paimai_item(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);
	// 保存拍卖信息
	BOOL insert_paimai(NET_S2DB_insert_paimai* p_paimai);
	// 读取所有拍卖信息
	BOOL load_all_paimai(LPVOID p_buffer);
	// 读取所有自动拍卖信息
	BOOL load_all_auto_paimai(LPVOID p_buffer);
	// 检测是否已经拍卖
	BOOL check_is_paimai(DWORD dw_auto_paimai_id);
	// 更新拍卖数据
	BOOL update_auto_paimai(NET_S2DB_update_auto_paimai* p_auto_paimai);
	// 读取拍卖物品
	BOOL load_paimai_item(LPVOID p_buffer);
	// 删除拍卖
	BOOL delete_paimai(NET_S2DB_delete_paimai* p_recv);
	// 更新拍卖
	BOOL update_paimai(NET_S2DB_update_paimai* p_recv);

private:
	VOID makesql_insert_paimai(odbc::sql_language_disposal* p_stream, DWORD dw_paimai_id, LPVOID p_buffer);
	VOID makesql_load_all_paimai(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_paimai_item(odbc::sql_language_disposal* p_stream, DWORD dw_paimai_id, DWORD dw_role_id);
	VOID makesql_delete_paimai(odbc::sql_language_disposal* p_stream, DWORD dw_paimai_id, LPVOID p_buffer);
	VOID makesql_update_paimai(odbc::sql_language_disposal* p_stream, DWORD dw_paimai_id, LPVOID p_buffer);
	VOID makesql_load_all_auto_paimai(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_update_auto_paimai(odbc::sql_language_disposal* p_stream, DWORD dw_paimai_id, LPVOID p_buffer);

private:
	VOID procres_laod_all_paimai(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_all_paimai_item(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_all_auto_paimai(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:
	// 获取钱庄最大编号
	BOOL	get_bank_max_id(DWORD&	dw_bank_id);
	// 保存钱庄信息
	BOOL	insert_bank(NET_S2DB_insert_bank* p_bank);
	// 删除钱庄信息
	BOOL	delete_bank(NET_S2DB_delete_bank* p_bank);
	// 更新钱庄
	BOOL	update_bank(NET_S2DB_update_bank* p_bank);
	// 读取所有钱庄信息
	BOOL	load_all_bank(LPVOID p_buffer);

private:
	VOID makesql_insert_bank(odbc::sql_language_disposal* p_stream, DWORD dw_bank_id, LPVOID p_buffer);
	VOID makesql_delete_bank(odbc::sql_language_disposal* p_stream, DWORD dw_bank_id, LPVOID p_buffer);
	VOID makesql_update_bank(odbc::sql_language_disposal* p_stream, DWORD dw_bank_id, LPVOID p_buffer);
	VOID makesql_load_all_bank(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id);
private:
	VOID procres_load_all_bank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// 排行榜

	// 获取等级排行榜
	BOOL load_level_rank();

	// 获取1v1积分榜
	BOOL load_1v1_score_rank();

	// 获取帮派排行榜
	BOOL load_guild_rank();

	// 获取装备排行榜
	BOOL load_equip_rank(OUT LPVOID &p_buffer, int *p_number);

	// 获取恶人排行榜
	BOOL load_kill_rank();
	
	// 获取正义榜
	BOOL load_justice_rank();
	//读取噬魂榜
	BOOL load_shihun_rank();
	//成就点数榜
	BOOL load_achpoint_rank();
	//成就数量榜
	BOOL load_achnumber_rank();
	// 师徒榜 gx add 2013.12.06
	BOOL load_master_rank();
	//坐骑榜
	BOOL load_mounts_rank();
	//充值榜
	BOOL load_reach_rank();
private:
	VOID makesql_load_level_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_guild_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_equip_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_kill_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_justice_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_1v1_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_shihun_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_achpoint_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_achnumber_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
	//师徒
	VOID makesql_load_master_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_mounts_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_load_reach_rank(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, DWORD dw_account_id);
private:
	VOID procres_load_level_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_guild_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_kill_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_justice_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_1v1_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_shihun_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_achpoint_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_achnumber_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	//师徒
	VOID procres_load_master_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_mounts_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_reach_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
public://名片

	// 获取角色名片
	BOOL load_visiting_card(LPVOID &p_buffer, DWORD dw_role_id);

	// 新增角色名片
	BOOL insert_visiting_card(DWORD dw_role_id);

	// 更新角色名片
	BOOL update_visiting_card(DWORD dw_role_id, const PVOID p_buffer);

	// 删除角色名片
	BOOL delete_visiting_card(DWORD dw_role_id);
private:
	VOID makesql_insert_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, PVOID pDum);
	VOID makesql_update_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_load_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
private:
	VOID procres_load_visiting_card(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public://家族
	
	// 获取角色家族数据
	BOOL load_clan_data(LPVOID &p_buffer, DWORD dw_role_id);

	// 新增家族数据
	BOOL insert_clan_data(DWORD dw_role_id);

	// 删除家族数据
	BOOL delete_clan_data(DWORD dw_role_id);
private:
	VOID makesql_insert_clan_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_update_clan_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_delete_clan_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_load_clan_data(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
private:
	VOID procres_load_clan_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:
	BOOL	insert_sign_data(DWORD dw_role_id);
	BOOL	insert_huenjing_data(DWORD dw_role_id);
	BOOL	insert_reward_data(DWORD dw_role_id);
	BOOL	insert_mounts_data(DWORD dw_role_id);
private:
	VOID makesql_insert_sign_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_insert_huenjing_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_insert_reward_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_insert_mounts_data(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
public:	// 名人堂

	// 家族珍宝
	BOOL load_act_treasures(LPVOID& p_buffer, int& n_total_num);

	// 入伙名人堂记录
	BOOL load_fame_hall_snaps(LPVOID& p_buffer, int& n_total_num);

	// 声望重置时间
	BOOL load_rep_reset_timestamp(LPVOID& p_buffer);

	// 声望排名榜
	BOOL load_rep_rank(BYTE b_clan_type, PVOID p_buffer, int &n_count);

	// 新增进入名人堂记录
	BOOL insert_fame_hall_enter_snap(PVOID p_buffer);

	// 新增家族珍宝
	BOOL insert_act_treasure(PVOID p_buffer);

	// 更新声望重置时间
	BOOL update_rep_reset_time(BYTE b_clan_type, DWORD dw_reset_time);
private:
	VOID makesql_load_act_treasures(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_fame_hall_snaps(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_reset_timestamp(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_rep_rank(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_insert_act_treasure(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_insert_fame_hall_enter_snap(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_load_rep_reset_timestamp(odbc::sql_language_disposal *p_stream, DWORD dwClanType, LPVOID p_buffer);
private:
	VOID procres_load_act_treasures(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_fame_hall_snaps(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_rep_reset_timestamp(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_rep_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public://帮派属性

	// 读取所有帮派资料
	BOOL load_all_guild(LPVOID p_buffer, int& n_guild_count);
	
	// 读取种植信息
	BOOL load_all_guild_plant_data(LPVOID p_buffer, int& n_count);

	// 新建帮派
	BOOL insert_guild(s_create_guild_info* p_create_info);

	// 删除帮派
	BOOL delete_guild(DWORD dw_guild_id);

	// 重置帮会报名数据
	BOOL resetSignUpAttack();

	// 修改帮派信条
	BOOL update_guild_tenet(DWORD dw_guild_id, LPTSTR sz_tenet);

	// 修改帮派图标
	BOOL update_guild_symbol(DWORD dw_guild_id, LPTSTR sz_tenet);

	// 保存帮派所有属性
	BOOL update_guild_all(tagGuildBase *p_guild_base, int n_guild_num);
	BOOL update_guild_all_plant(s_guild_plant *p_guild_plant, int n_guild_num);

	// 保存帮派属性
	BOOL update_guild_attribute(tagGuildBase *p_guild_base);
	BOOL update_guild_plant(s_guild_plant* p_guild_plant);
	BOOL load_sbk_guild(LPVOID p_buffer, int& n_count);
	BOOL save_guild_sbk(DWORD dw_guild_id);
	// 修改帮派职位名称
	BOOL update_guild_pos_name(DWORD dw_guild_id, NET_DB2C_change_guild_pos_name *p_guild_pos_name);
	
	// 修改帮派职位权利
	BOOL update_guild_pos_power(DWORD dw_guild_id, NET_DB2C_change_guild_pos_power *p_guild_pos_power);	
private:
	VOID makesql_load_all_guild(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_all_guild_plant(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_insert_guild(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_guild(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_reset_sign_up_attack(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_guild_tenet(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_symbol(odbc::sql_language_disposal* p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_all(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_plant(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_pos_name(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_pos_power(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_save_guild_sbk(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_load_sbk_guild(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id);
private:
	VOID procres_load_all_guild(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_all_guild_plant(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_sbk_guild(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public://帮派成员属性

	// 获取所有帮派成员资料
	BOOL load_all_guild_member(LPVOID p_buffer, int& n_guil_member);

	// 新加帮派成员
	BOOL insert_guild_member(const s_create_guild_member_info& create_member_info);

	// 帮派成员被踢或者自己离帮
	BOOL delete_guild_member(DWORD dw_role_id);

	// 更新帮派职位
	BOOL update_guild_member_pos(DWORD dw_role_id, INT8 n8_pos);

	// 设置帮派成员访问仓库权限
	BOOL guild_ware_privilege(DWORD dw_role_id, BOOL b_enable);

	// 更改帮派成员贡献度
	BOOL change_member_contribution(LPVOID p_buffer);

	// 设置帮派成员投票权限
	BOOL change_ballot(LPVOID p_buffer);
	
	// 设置是否参战
	//BOOL change_war(LPVOID p_buffer);

	// 更新帮派成员DKP
	BOOL change_DKP(LPVOID p_buffer);
private:
	VOID makesql_load_all_guild_member(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID maeksql_insert_guild_member(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_guild_member(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_guild_member_pos(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_guild_ware_privilege(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_change_member_contribution(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_change_ballot(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	//VOID makesql_change_war(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_change_DKP(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID procres_load_all_guild_member(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// 帮派仓库

	// 获取帮派仓库所有道具
	BOOL load_guild_ware_all_items(LPVOID &p_buffer, DWORD dw_guild_id, int& n_item);
private:
	VOID makesql_load_guild_ware_items(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id);

public:// 帮派设施

	// 获取帮派所有设施
	BOOL load_guild_facilities(LPVOID p_buffer, int& n_info, DWORD dw_guild_id);

	// 新增帮派设施
	BOOL insert_guild_facilities(s_facilities_load* p_info);

	// 更新帮派设施
	BOOL update_guild_facilities(s_facilities_load* p_info);

	// 删除帮派设施
	BOOL delete_guild_facilities(DWORD dw_guild_id);
private:
	VOID makesql_load_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id);
	VOID makesql_insert_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
private:
	VOID procres_load_guild_facilities(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public://帮派pvp

	// 新建帮派PVP数据
	BOOL insert_guild_pvp_data(int n_act_id, NET_DB2C_insert_pvp_data* p_pvp);

	// 取得帮派PVP数据
	BOOL load_guild_pvp_data(LPVOID p_buffer, int& nDataNum);

	// 更新帮派PVP数据
	BOOL update_pvp_data(int n_act_id, NET_DB2C_up_pvp_data* p_pvp);
private:
	VOID makesql_insert_guild_pvp_data(odbc::sql_language_disposal* p_stream, DWORD dwActID, LPVOID p_buffer);
	VOID makesql_load_guild_pvp_data(odbc::sql_language_disposal* p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_update_pvp_data(odbc::sql_language_disposal* p_stream, DWORD dwActID, LPVOID p_buffer);
private:
	VOID procres_load_guild_pvp_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:
	// 读取帮会招募榜信息
	BOOL load_guild_recruit(LPVOID p_buffer);
	// 添加帮会招募信息
	BOOL insert_guild_recruit(DWORD dw_role_id);
	// 删除帮会招募信息
	BOOL delete_guild_recruit(DWORD dw_role_id);
	// 清除帮会招募榜
	BOOL clean_guild_recruit();
private:
	VOID makesql_load_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_insert_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_clean_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);

private:
	VOID procres_load_guild_recruit(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:
	// 读取帮会boss击杀
	BOOL load_guild_boss_kill(LPVOID p_buffer);
	BOOL insert_guild_kill_boss(s_guild_kill_boss* p_data);
	// 帮会战绩
	BOOL load_guild_war_history(LPVOID p_buffer);	
	BOOL insert_guild_war_history(tagGuildWarHistory* p_data);
	BOOL delete_guild_war_history(tagGuildWarHistory* p_data);
private:
	VOID makesql_load_guild_boss_kill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_insert_guild_boss_kill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_load_guild_war_history(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_insert_guild_war_history(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_guild_war_history(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);

private:
	VOID procres_load_guild_boss_kill(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_guild_war_history(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// 帮派技能

	// 获取帮派所有技能
	BOOL load_guild_skill_info(LPVOID p_buffer, DWORD dw_guild_id, int& n_count);

	// 新增帮会技能
	BOOL insert_guild_skill_info(DWORD dw_guild_id, s_guild_skill_info* pSkillInfo);

	// 更新帮派技能
	BOOL update_guild_skill_info(DWORD dw_guild_id, s_guild_skill_info* pSkillInfo);

	// 更改帮派研究技能
	BOOL change_research_guild_skill(DWORD dw_guild_id, DWORD dw_skill_id);

	// 删除帮会技能
	BOOL delete_guild_skill_info(DWORD dw_guild_id);
private:
	VOID makesql_load_guild_skill_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id);
	VOID makesql_insert_guild_skill_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_skill_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_change_research_guild_skill(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID pSkillID);
	VOID makesql_clear_research_guild_skill(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID pSkillID);
	VOID makesql_delete_guild_skill_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID pSkillID);
private:	
	VOID procres_load_guild_skill_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// 帮派弹劾

	// 获取帮派弹劾
	BOOL load_guild_delate(LPVOID p_buffer, DWORD dw_guild_id);

	// 新增帮派弹劾
	BOOL insert_guild_delate(DWORD dw_guild_id);

	// 删除帮派弹劾
	BOOL delete_guild_delate(DWORD dw_guild_id);

	// 更新帮派弹劾
	BOOL update_guild_delate(LPVOID p_buffer, DWORD dw_guild_id);

	// 更新帮派弹劾内容
	BOOL update_guild_delate_content(DWORD dw_guild_id, LPTSTR szNewTenet);
private:
	VOID makesql_load_guild_delate(odbc::sql_language_disposal * p_stream, DWORD dw_guild_id, DWORD dw_role_id);
	VOID makesql_insert_guild_delate(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_delete_guild_delate(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_delate(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_delate_content(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
private:
	VOID procres_load_guild_delate(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// 帮派跑商

	// 获取帮派商会资料
	BOOL load_guild_commerce_info(LPVOID p_buffer, DWORD dw_guild_id, int& n_count);

	// 获取帮派商会排行榜
	BOOL load_commerce_rank_info(LPVOID p_buffer, DWORD dw_guild_id, int& n_count);

	// 新增商品信息
	BOOL insert_commodity_info(LPVOID p_buffer);

	// 更新商品金钱
	BOOL update_commodity_money(DWORD dw_role_id, int n_money);

	// 保存商品信息
	BOOL save_commodity_info(LPVOID p_buffer, DWORD dw_role_id);

	// 删除商品信息
	BOOL delete_commodity_info(DWORD dw_role_id);

	// 设置帮派表扬
	BOOL set_guild_citation(DWORD dw_guild_id, BOOL b_citation);

	// 更新商会排名
	BOOL update_commerce_rank_info(DWORD dw_guild_id, LPVOID p_buffer);

	// 删除商会排名
	BOOL delete_commerce_rank_info(DWORD dw_guild_id, DWORD dw_role_id);
private:
	VOID makesql_load_guild_commerce_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id);
	VOID makesql_load_commerce_rank_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id);
	VOID makesql_insert_commodity_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_commodity_money(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID nTael);
	VOID makesql_save_commodity_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_commodity_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_set_guild_citation(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_commerce_rank_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_delete_commerce_rank_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
private:
	VOID procres_load_guild_commerce_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_commerce_rank_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// 宠物相关
	BOOL load_one_pet(OUT PVOID& p_buffer, DWORD dw_pet_id);
	BOOL load_pet_soul(LPVOID p_buffer, DWORD dw_pet_id);
	BOOL load_pet_skill(OUT LPVOID& p_buffer, DWORD dw_pet_id, int& n_count);
	BOOL load_role_all_pet_id(DWORD pPetIDs[], DWORD dw_master_id, int& n_count);
	BOOL load_pet_equip(OUT LPVOID& p_buffer, DWORD dw_pet_id, int& n_count);
	BOOL insert_pet_soul(const s_db_pet_soul_create* p_create, DWORD& dw_pet_id);
	BOOL delete_pet_soul(DWORD dw_pet_id);
	BOOL delete_pet_equip(DWORD dw_pet_id);
	BOOL insert_pet_skill(PVOID p_buffer, DWORD dw_pet_id);
	BOOL update_pet_skill(PVOID &p_buffer, const int n_count, DWORD dw_pet_id);
	BOOL delete_pet_skill(DWORD dwPetSkillID, DWORD dw_pet_id);
	BOOL load_pets(OUT PVOID& p_buffer, DWORD dw_master_id, int& n_count);
	BOOL save_pets(DWORD dw_role_id, IN LPVOID p_buffer, int n_count, OUT LPVOID* pp_buffer);
	BOOL save_one_pet(OUT PVOID& p_buffer, DWORD dw_pet_id);
	BOOL update_pet_attribute( const s_db_pet_att* p_att, DWORD& dw_pet_id);
private:
	VOID makesql_load_pet_soul(odbc::sql_language_disposal *p_stream, DWORD dwPetID, DWORD dwMasterID);
	VOID makesql_load_pet_skill(odbc::sql_language_disposal *p_stream, DWORD dwMasterID, DWORD dwPetID);
	VOID makesql_load_role_all_pet_id(odbc::sql_language_disposal *p_stream, DWORD dwPetID, DWORD dwMasterID);
	VOID makesql_load_pet_equip(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dwPetID);
	VOID makesql_insert_pet_soul(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_pet_soul(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_insert_pet_skill(odbc::sql_language_disposal *p_stream, DWORD dwPetID, LPVOID p_buffer);
	VOID makesql_update_pet_skill(odbc::sql_language_disposal *p_stream, DWORD dwPetID, LPVOID p_buffer);
	VOID makesql_delete_pet_skill( odbc::sql_language_disposal *p_stream, DWORD dwPetID, LPVOID p_buffer );
	VOID makesql_update_pet_attribute( odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer );
private:
	VOID procres_load_pet_soul(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_pet_skill(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_role_all_pet_id(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// 商城相关
	
	// 获取所有团购信息
	BOOL load_all_group_purchase_info(OUT PVOID p_buffer, int &n_info_number);

	// 新增团购信息
	BOOL add_group_purchase_info(s_gp_info* p_gp_info);

	// 删除团购信息
	BOOL delete_group_purchase_info(s_gp_info_key* p_gp_info_key);

	// 删除帮派团购信息
	BOOL delete_guild_group_purchase_info(DWORD dw_guild_id);

	// 更新团购信息
	BOOL update_group_purchase_info(s_gp_info_key* p_gp_info_key, DWORD dwNewParticipator);

	// 更新团购时间
	BOOL update_group_purchase_time();
private:
	VOID makesql_load_all_group_purchase_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_add_group_purchase_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_group_purchase_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_guild_group_purchase_info(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_group_purchase_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_group_purchase_time(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID procres_load_all_group_purchase_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// VIP摊位
	// 获取VIP摊位
	BOOL get_all_vip_stall_info(OUT PVOID p_buffer);

	// VIP摊位申请
	BOOL apply_vip_stall(s_vip_stall_to_db* pInfo);

	// VIP摊位剩余时间
	BOOL update_vip_stall_time();
private:
	VOID makesql_get_all_vip_stall_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_apply_vip_stall(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_vip_stall_time(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_insert_vip_stall(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID procres_get_all_vip_stall_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// 金牌网吧
	BOOL update_vip_net_bar_player(DWORD dw_account_id, DWORD dw_login_time);
	BOOL load_vip_net_bar(OUT PVOID p_buffer, DWORD dw_date);
private:
	VOID makesql_load_history_vip_net_bar_players(odbc::sql_language_disposal *p_stream, DWORD dw_date, DWORD dw_data);
	VOID makesql_load_today_vip_net_bar_players(odbc::sql_language_disposal *p_stream, DWORD dw_date, DWORD dw_data);
	VOID makesql_update_vip_net_bar_player(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, PVOID p_buffer);
private:
	VOID procres_load_vip_net_bar_players(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// 留言

	// 读取留言
	BOOL load_leave_msg(DWORD dw_role_id, PVOID p_leave_msg, DWORD &dw_size);

	// 获取留言最大索引
	BOOL load_leave_msg_indexes(DWORD* p_indexes, DWORD dw_role_id, int &n_count);

	// 删除留言
	BOOL delete_leave_msg(DWORD dw_role_id, DWORD dwDateTime);

	// 新增留言
	BOOL insert_leave_msg(DWORD dw_role_id, DWORD dwDateTime, PVOID pLeftMsg);

	// 清空留言
	BOOL clear_leave_msg(DWORD dw_role_id);
private:
	VOID makesql_load_leave_msg(odbc::sql_language_disposal *p_stream, DWORD dwDumy, DWORD dw_role_id);
	VOID makesql_load_leave_msg_indexes(odbc::sql_language_disposal *p_stream, DWORD dwDumy, DWORD dw_role_id);
	VOID makesql_delete_leave_msg(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID pDumy);
	VOID makesql_insert_leave_msg(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_clear_leave_msg(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID procres_load_leave_msg(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_leave_msg_indexes(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public://开启宝箱统计

	// 更新宝箱数目
	BOOL update_treasure_count(DWORD dw_role_id, int& nSum);
private:
	VOID makesql_update_treasure_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, LPVOID p_buffer);

public: //师徒系统

	// 获取师徒数据
	BOOL load_all_master_prentices( OUT LPVOID& pBuffer, OUT int& n32Size );

	// 获取师徒榜
	BOOL load_all_master_recruit(LPVOID pBuffer, OUT INT& n32Size);

	// 保存师徒数据
	BOOL save_master_prentices(LPVOID p_buffer, DWORD dw_role_id, UINT32 u32Number);

	// 将角色加入师徒榜
	BOOL add_new_master_prentices( DWORD dw_role_id, LPVOID p_buffer = NULL );

	// 更新师傅ID和禁止操作时间
	BOOL update_master_id_and_forbid_time( DWORD dw_role_id, LPVOID p_buffer );

	// 师傅删除角色
	BOOL master_delete_role( DWORD dw_role_id, LPVOID p_buffer = NULL );

	// 加入拜师榜
	BOOL join_master_recruit(DWORD dw_role_id, LPVOID p_buffer = NULL);

	// 退出拜师榜
	BOOL leave_master_recruit(DWORD dw_role_id, LPVOID p_buffer = NULL);
private:
	VOID makesql_load_all_master_prentices(odbc::sql_language_disposal* p_stream, DWORD dwAccoun, DWORD dwRole );
	VOID makesql_load_all_master_recruit(odbc::sql_language_disposal* p_stream, DWORD dwAccoun, DWORD dwRole );
	VOID init_load_master_prentices_fields(odbc::sql_language_disposal* p_stream );
	VOID makesql_save_master_prentices(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_add_new_master_prentices(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_master_id_and_forbid_time(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_master_delete_role(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_join_master_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_leave_master_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID procres_load_all_master_prentices(OUT LPVOID &p_buffer, OUT int *pSize, odbc::execute_result *p_result);
	VOID procres_load_all_master_recruit(OUT LPVOID &p_buffer, OUT int *pSize, odbc::execute_result *p_result);

public:// 活动系统

	// 读取所有活动数据
	BOOL load_activities(LPVOID p_buffer, int &n_count);

	// 存储某个活动数据
	BOOL save_activity(DWORD dw_id, LPVOID pActivityData);
private:
	VOID makesql_save_activity(odbc::sql_language_disposal *p_stream, DWORD dw_id, LPVOID p_activity_data);
	VOID makesql_load_activities(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
private:
	VOID procres_load_activities(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// 日志记录

	// 金钱日志
	BOOL create_log_money_table(LPCSTR sz_table_postfix);

	// 元宝日志
	BOOL create_log_yuanbao_table(LPCSTR sz_table_postfix);

	// 赠点日志
	BOOL create_log_exvolume_table(LPCSTR sz_table_postfix);

	// 战功日志
	BOOL create_log_exploits_table(LPCSTR sz_table_postfix);

	// 定时日志
	BOOL create_log_time_stat_table(LPCSTR sz_table_postfix);

	// 物品日志
	BOOL create_log_item_table(LPCSTR sz_table_postfix);

	// 物品次数日志
	BOOL create_log_item_times_table(LPCSTR sz_table_postfix);

	// 商城出售日志
	BOOL create_log_mall_sell_table(LPCSTR sz_table_postfix);

	// 商城出售的礼包日志
	BOOL create_log_mall_sell_pack_table(LPCSTR sz_table_postfix);

	// 帮派基金日志
	BOOL create_log_fund_table(LPCSTR sz_table_postfix);

	// 材料日志
	BOOL create_log_material_table(LPCSTR sz_table_postfix);

	//声望日志
	BOOL create_log_reputation_table(LPCSTR sz_table_postfix);

	// GM 指令日志
	BOOL create_log_gm_cmd_table(LPCSTR sz_table_postfix);
	
	// 钱庄日志
	BOOL create_log_bank_table(LPCSTR sz_table_postfix);

	// 拍卖日志
	BOOL create_log_paimai_table(LPCSTR sz_table_postfix);

	// 离线修炼日志
	BOOL create_log_leave_practice_table(LPCSTR sz_table_postfix);
	// 序列号礼包
	BOOL create_log_serial_reward(LPCSTR sz_table_postfix);

	// 创建踢人日志
	BOOL create_log_kick(LPCSTR sz_table_postfix);
public:
	// 新增日志记录
	VOID insert_money_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_yuanbao_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_exvolume_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_exploits_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_time_stat_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_item_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_item_times_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_mall_sell_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_mall_sell_pack_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_fund_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_material_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_reputation_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_gm_cmd_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_bank_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_paimai_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_leave_practice_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_serial_reward(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID insert_kick_log(LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID undate_online_num(LPVOID p_buffer);
private:
	VOID makesql_insert_money_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_yuanbao_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_exvolume_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID madesql_insert_exploits_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_time_stat_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_item_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_item_times_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_mall_sell_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_mall_sell_pack_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_fund_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_material_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_reputation_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_gm_cmd_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_bank_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_paimai_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_leave_pracitce_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_serial_reward(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
	VOID makesql_insert_kick_log(odbc::sql_language_disposal *p_stream, LPCSTR sz_table_postfix, LPVOID p_buffer);
private:
	// 创建日志表方法
	BOOL common_create_log_table(LPCSTR str_table, LPCSTR sz_table_postfix);
public:
	// 重置玩家元气值
	BOOL reset_all_role_vigour( DWORD dw_role_id, LPVOID p_buffer );
	// 重置幸运
	BOOL clean_all_role_luck( DWORD dw_role_id, LPVOID p_buffer );

	BOOL clear_destory_equip_count( DWORD dw_role_id, LPVOID p_buffer );

	BOOL clean_shihun(DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID makesql_reset_all_role_vigour(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_clean_all_role_luck(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_clear_destory_equip_count(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_clean_shihun(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
public:
	// 1v1积分竞技场
	BOOL update_role_1v1_score(DWORD dw_role_id, LPVOID p_buffer);
	BOOL update_noline_1v1_score(DWORD dw_role_id, LPVOID p_buffer);

	BOOL update_mounts(DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID makesql_update_role_1v1_score(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_noline_1v1_score(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);

	VOID makesql_update_mounts(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
public:
	// 悬赏任务
	BOOL load_all_guerdonquest(OUT LPVOID p_buffer, OUT INT& nNumber);
	BOOL load_all_gdreward(OUT LPVOID& p_buffer, OUT INT& nNumber);
	VOID insert_new_guerdonquest(PVOID p_buffer);
	VOID update_guerdonquest(PVOID p_buffer);
	VOID delete_guerdonquest(PVOID p_buffer);
	VOID update_questcompletedata(PVOID p_buffer);
	VOID update_quest_flag(PVOID p_buffer);
	VOID insert_quest_rewarditem(PVOID p_buffer);
	VOID delete_quest_rewarditem(PVOID p_buffer);

	// 宠物sns
	BOOL insert_pet_sns_info(const tagPetSNSInfo* p_buffer);
	BOOL delete_pet_sns_info(DWORD dw_pet_id);
	BOOL load_pet_sns_info(OUT PVOID p_buffer, OUT INT& nNumber);
private:
	VOID makesql_load_all_guerdonquest(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_load_all_guerdonquest_reward(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_insert_new_guerdonquest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_guerdonquest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_guerdonquest(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_questcompletedata(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_quest_flag(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_insert_quest_rewarditem(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_quest_rewarditem(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);

	VOID makesql_insert_pet_sns_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_pet_sns_info(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_load_pet_sns_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID procres_load_pet_sns_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
private:
	VOID procres_load_all_guerdonquest(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
public:
	BOOL inc_circle_refreshnumber_and_questnum( DWORD dw_role_id, LPVOID p_buffer );
private:
	VOID makesql_inc_circle_refreshnumber_and_questnum(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
private:
	// 获取roledata记录条数和最大RoleID
	VOID get_role_count_and_max_role_id(int &n_count, DWORD &dw_max_role_id);

	// 获取宠物总数和最大ID
	VOID get_pet_count_and_max_pet_id(int &n_count, DWORD &dw_max_pet_id);

	// 获取元宝账户数量
	VOID get_yuanbao_account_count(int &n_account_count);

	// 获取元宝订单数量
	VOID get_yuanbao_order_count(int &n_order_count);

	// 获得活动数量
	VOID get_activity_count(int &n_activity_count);

	// 获取最大和最小序列号
	VOID get_world_max_min_serial(const char* sz_table, int64 &n64Max, int64 &n64Min);

	// 获取邮件最大ID
	VOID get_max_mail_serial(const char* sz_table, DWORD& dwMax);

	// 获取团购总数和数据大小
	VOID get_group_purchase_info_count_size(int &n_gp_info_number, int &n_data_size);

public:
	// 读数据库次数
	DWORD get_total_read_count() const { return dw_read_count_; }

	// 写数据库次数
	DWORD get_total_write_count() const { return dw_write_count_; }

	// 获得角色总数
	int get_role_count() const { return n_role_count_; }

	// 获得元宝账户总数
	int get_account_count() const { return n_account_count_; }

	// 获得元宝订单总数
	int get_yuanbao_order_count() const { return n_order_count_; }

	// 获得元宝订单最大ID
	DWORD get_yuanbao_max_order_id() const	{ return dw_max_order_index_; }

	// 获得元宝订单最大索引
	VOID get_yuanbao_max_order_index(DWORD& dw_max_index);

	// 获得角色元宝订单数
	VOID get_one_role_yuanbao_order_count(DWORD dw_role_id, int &n_order_number);

	// 获得活动总数
	int get_activity_count() const { return n_activity_count_; }

	// 修正物品世界唯一最大/最小索引
	VOID get_world_max_min_serial(int64 &n64_max, int64 &n64_min);

	int get_group_purchase_info_count() const { return n_group_purchase_info_count_; }
	int get_group_purchate_info_size() const { return n_group_purchase_info_size_; }

	VOID reset_load_count_limit(int n_load_limit)	{ n_load_count_limit_ = n_load_limit; }
	VOID reset_load_serial(int64 n64_serial)	{ n64_load_baibao_item_serial = n64_serial; }

public:// 数据库压力测试
	BOOL force_test_load_role(LPVOID &p_role_data, DWORD dw_account_id, DWORD dw_role_id)
	{
		return load_one_role_data(p_role_data, dw_account_id, dw_role_id);
	}
	BOOL force_test_load_item(LPVOID &p_buffer, int &n_count, DWORD dw_account_id, DWORD dw_role_id)
	{
		return load_one_role_items(p_buffer, n_count, dw_account_id, dw_role_id);
	}


public: // 表(解除)锁定
	BOOL lock_tables(LPCSTR szTableNames);
	BOOL unlock_tables();
private:
	VOID makesql_lock_tables(odbc::sql_language_disposal *p_stream, DWORD dw_reserve, LPVOID p_buffer);
	VOID makesql_unlock_tables(odbc::sql_language_disposal *p_stream, DWORD dw_reserve, LPVOID p_buffer);

private: 
	// 请求结果集,同步操作
	BOOL common_select(
						OUT LPVOID &p_buffer, 
						OUT int *p_number, 
						DWORD dw_account_id, 
						DWORD dw_role_id, 
						func_param3 p_func_format, 
						func_result_proc p_func_procres
	);

	// 更新数据(删除/插入/更新),不需结果,同步操作
	BOOL common_update(
						DWORD dw_role_id, 
						LPVOID p_buffer, 
						int n_count, 
						int nSzOne, 
						func_param3_1 p_func_format, 
						OUT LPVOID *pp_buffer = NULL
	);

	// 日志操作,不需结果集,异步操作(删除/插入/更新)
	VOID common_update_log(
							LPCSTR sz_table_postfix, 
							LPVOID p_buffer, 
							func_param3_2 p_func_format
	);
private:
	// 初始化物品读物字段
	VOID init_item_load_field(odbc::sql_language_disposal *p_stream);

private:
	
	few_connect_server* server_;
	db_server*			p_db_server_;

	// 角色总数
	int	n_role_count_;
	// 最大角色ID
	DWORD dw_max_role_id_;
	// 账号总数
	int	n_account_count_;

	// 订单总数
	int	n_order_count_;
	// 最大订单索引
	DWORD dw_max_order_index_;
	// 一次读取最大数
	int   n_load_count_limit_;
	// 百宝袋中物品最大ID
	int64 n64_load_baibao_item_serial;

	// 团购最大数
	int	n_group_purchase_info_count_;
	// 团购信息内存大小
	int	n_group_purchase_info_size_;

	// 最大宠物ID
	DWORD dw_max_pet_id_;
	// 宠物个数
	int n_pet_count_;

	// 活动总数
	int n_activity_count_;

	// 拼合SQL语句时间时使用的buffer
	char sz_time_buffer_[X_DATATIME_LEN + 1];
	// 角色名CRC32时使用
	TCHAR sz_role_name_buffer_[64];

	// 是否已经连接数据库
	volatile BOOL b_is_init_ok_;
	// 读数据库次数
	volatile DWORD dw_read_count_;
	// 写数据库次数
	volatile DWORD dw_write_count_;

	// 游戏服务器的数据库接口
	odbc::db_interface*	game_db_interface_;
	// 日志数据库接口
	odbc::db_interface*	log_db_interface_;
	// 人数数据库接口
	odbc::db_interface* user_number_db_interface;
};
 
#endif //__DATABASE_HANDLRR_H__
