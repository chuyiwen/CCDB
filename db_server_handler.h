/*******************************************************************************

Copyright 2010 by tiankong Interactive Game Co., Ltd.
All rights reserved.

This software is the confidential and proprietary information of
tiankong Interactive Game Co., Ltd. ('Confidential Information'). You shall
not disclose such Confidential Information and shall use it only in
accordance with the terms of the license agreement you entered into with
tiankong Interactive  Co., Ltd.

*******************************************************************************/

/**
 *	@file		db_server_handler
 *	@author		mwh
 *	@date		2010/10/11	initial
 *	@version	0.0.1.0
 *	@brief		数据库服务器类
*/


#ifndef __DATABASE_SERVER_HANDLER_H__

#define __DATABASE_SERVER_HANDLER_H__

#include "../common/ServerDefine/base_server_define.h"

//前置声明
struct NET_DB2S_load_role;
struct NET_DB2S_load_simrole;
struct NET_DB2S_create_role;
struct tagNDBS_SaveRole;
struct NET_DB2S_load_baibao;
struct NET_DB2S_load_baibao_log;
struct NET_DB2S_get_all_gp_info;

class db_handler;

//数据库消息缓冲队列,先进先出队列
//只做存储，不负责内存管理，所以外部调用需要负责相应的内存管理
class db_message_queue
{
public:
	db_message_queue()	
	{ 
		//手动重置
		event_handle_ = ::CreateEvent(NULL, FALSE, TRUE, NULL);
	}

	~db_message_queue()	{ ::CloseHandle(event_handle_); }

public:
	// 在尾部添加一个消息
	VOID add_msg(LPBYTE p_msg)
	{
		msg_queue_.push_back(p_msg);
		
		// 激活在此事件上等待的所有线程
		::SetEvent(event_handle_);
	}

	// 取得第一个消息包，外部调用者需要管理相应内存
	LPBYTE	get_msg(){ return msg_queue_.pop_front(); }

	// 获取handle
	HANDLE	get_event() { return event_handle_; }

	// 缓存的消息总数
	int		msg_count() { return msg_queue_.size(); }

private:
	// 消息队列
	package_safe_list<LPBYTE>	msg_queue_;
	// 同步事件
	HANDLE				event_handle_;
};

// 数据库服务器
class db_server
{
public:
	db_server();
	~db_server();

public:
	static file_container* s_p_var;
	static thread_manager*		p_thread;
	// 启动
	BOOL start(HINSTANCE hInst);
	// 主循环
	VOID main_loop();
	// 关闭线程
	VOID shutdown() { Interlocked_Exchange((long*)(&b_terminate_), TRUE); }
public:
	// 需要关闭
	BOOL is_terminate() const { return b_terminate_; }
	// 是否连接
	BOOL is_connect() const { return !b_server_disconnect_ && b_has_server_connect_; }
	// 当前心跳
	DWORD get_heartbeat() const { return get_status( ).dw_heartbeat_; }

	file_system* get_file_system() { return p_vfs; }
private: //网络消息处理接口

	// 获取所有玩家基本数据
	DWORD load_all_role_simple(DWORD p_msg, DWORD dw_reserve);

	// 获取角色简单数据
	DWORD load_one_role_simple(DWORD p_msg, DWORD dw_reserve);

	// 获取角色完整数据
	DWORD load_one_role_full_data(DWORD p_msg, DWORD dw_reserve);

	// 获取账号序列号礼包
	DWORD load_serial_reward(DWORD p_msg, DWORD dw_reserve);

	// 存储角色
	VOID save_one_role(DWORD p_msg, DWORD dw_reserve);

	//gx add 2013.9.17 及时更新玩家奖励数据
	VOID update_one_role_reward_data(DWORD p_msg,DWORD dw_reserve);

	// 创建角色
	DWORD create_role(DWORD p_msg, DWORD dw_reserve);

	// 删除角色
	DWORD delete_role(DWORD p_msg, DWORD dw_reserve);

	// 恢复删除角色保护时间
	DWORD role_guard_cancel_time(DWORD p_msg, DWORD dw_reserve);

	// 设置删除角色保护时间
	DWORD role_guard_set_time(DWORD p_msg, DWORD dw_reserve);

	// 重新激活删除的角色
	DWORD active_role(DWORD p_msg, DWORD dw_reserve);
	
	// 角色改名
	DWORD change_role_name(DWORD p_msg, DWORD dw_reserve);

	// 角色更改帮会
	DWORD change_role_guild(DWORD p_msg, DWORD dw_reserve);

	// 角色离开帮会时间
	DWORD update_role_leave_guild_time(DWORD p_msg, DWORD dw_reserve);

	// 更新角色VIP信息 gx add 2013.8.14
	DWORD update_role_vip_info(DWORD p_msg,DWORD dw_reserver);
	
	// 角色穿着模型ID
	DWORD change_role_dress_model_id(DWORD p_msg, DWORD dw_reserve);

	// 按类型删除副本进度
	DWORD delete_map_process(DWORD p_msg, DWORD dw_reserve);

	// 更新进入限制
	DWORD update_role_map_limit(DWORD p_msg, DWORD dw_reserve);

	// 删除进入限制
	DWORD delete_role_map_limit(DWORD p_msg, DWORD dw_reserve);

	// 新增进入限制
	DWORD insert_role_map_limit(DWORD p_msg, DWORD dw_reserve);

	// 游戏服务器初始化成功
	DWORD game_server_init_ok(DWORD p_msg, DWORD dw_reserve);

	// 游戏服务器关闭
	DWORD game_server_showdown(DWORD p_msg, DWORD dw_reserve);
	
	// 向GameServer返回初始化完成
	DWORD game_server_ok(DWORD p_msg, DWORD dw_reserve);

	// 修正物品世界唯一最大/最小索引
	DWORD get_world_max_min_serial(DWORD p_msg, DWORD dw_reserve);	

	// 获取百宝物品
	DWORD load_item_baibao(DWORD p_msg, DWORD dw_reserve);

	// 读取百宝袋中元宝信息
	DWORD load_baibao_yuanbao(DWORD p_msg, DWORD dw_reserve);

	// 读取网页奖励信息
	DWORD load_web_receive(DWORD p_msg, DWORD dw_reserver);

	// 更新元宝充值
	DWORD update_yuanbao_recharge(DWORD p_msg, DWORD dw_reserve);

	// 更新元宝充值总量
	DWORD update_total_recharge(DWORD p_msg, DWORD dw_reserve);

	// 删除百宝物品
	DWORD delete_baibao_item(DWORD p_msg, DWORD dw_reserve);

	// 从删除百宝装备
	DWORD delete_baibao_equip(DWORD p_msg, DWORD dw_reserve);

	// 获取需log的item类型
	DWORD load_item_need_log(DWORD p_msg, DWORD dw_reserve);

	// 获取元宝账户
	DWORD load_all_yuanbao_account(DWORD p_msg, DWORD dw_reserve);

	// 获取所有元宝订单
	DWORD load_all_yuanbao_order(DWORD p_msg, DWORD dw_reserve);

	// 新增元宝账户
	DWORD insert_yuanbao_account(DWORD p_msg, DWORD dw_reserve);

	// 更新账户元宝
	DWORD update_account_yuanbao(DWORD p_msg, DWORD dw_reserve);

	// 更新账户金钱
	DWORD update_account_silver(DWORD p_msg, DWORD dw_reserve);

	// 订单出售
	DWORD update_submit_sell(DWORD p_msg, DWORD dw_reserve);

	// 订单收购
	DWORD update_submit_buy(DWORD p_msg, DWORD dw_reserve);

	// 新增订单
	DWORD insert_one_order(DWORD p_msg, DWORD dw_reserve);

	// 完成订单
	DWORD complete_one_order(DWORD p_msg, DWORD dw_reserve);

	// 刷新订单
	DWORD refresh_one_order(DWORD p_msg, DWORD dw_reserve);

	// 读取角色元宝订单
	DWORD load_one_role_all_yuanbao_order(DWORD p_msg, DWORD dw_reserve);

	// 删除订单
	DWORD delete_one_order(DWORD p_msg, DWORD dw_reserve);

	// 获取百宝日志
	DWORD load_baibao_log_info(DWORD p_msg, DWORD dw_reserve);

	// 新增百宝日志
	DWORD insert_baibao_log_info(DWORD p_msg, DWORD dw_reserve);

	// 更新包裹非绑定币
	DWORD bag_meony_update(DWORD p_msg, DWORD dw_reserve);
	
	// 帐号属性
	DWORD set_safe_code(DWORD p_msg, DWORD dw_reserve);
	DWORD reset_safe_code(DWORD p_msg, DWORD dw_reserve);
	DWORD cancel_safe_code_reset(DWORD p_msg, DWORD dw_reserve);
	DWORD update_ware_size(DWORD p_msg, DWORD dw_reserve);
	DWORD update_ware_money(DWORD p_msg, DWORD dw_reserve);
	DWORD update_baibao_yuanbao(DWORD p_msg, DWORD dw_reserve);
	DWORD update_exchange_volume(DWORD p_msg, DWORD dw_reserve);
	DWORD update_account_receive(DWORD p_msg, DWORD dw_reserve);
	DWORD update_account_receive_ex(DWORD p_msg, DWORD dw_reserve);
	DWORD update_web_recieve(DWORD p_msg,	DWORD dw_reserve);
	DWORD delete_serial_reward(DWORD p_msg,	DWORD dw_reserve);

	DWORD change_bag_password(DWORD p_msg, DWORD dw_reserve);

	// 读取所有活动数据
	DWORD load_activities(DWORD p_msg, DWORD dw_reserve);

	// 存储某个活动数据
	DWORD save_activity(DWORD p_msg, DWORD dw_reserve);

	// 更新角色属性
	DWORD update_role_attribute(DWORD p_msg, DWORD dw_reserve);

	// 更新角色属性点
	DWORD update_role_attribute_point(DWORD p_msg, DWORD dw_reserve);

	// 更新角色天赋点
	DWORD update_role_talent_point(DWORD p_msg, DWORD dw_reserve);

	// 清除角色不在线经验
	DWORD clear_role_leave_exp(DWORD p_msg, DWORD dw_reserve);

	// 清除角色不在线义气值
	DWORD clear_role_leave_brotherhood(DWORD p_msg, DWORD dw_reserve);

	// 保存邮件
	DWORD save_mail(DWORD p_msg, DWORD dw_reserve);

	// 保存邮件物品
	DWORD update_mail_item(DWORD p_msg, DWORD dw_reserve);

	// 获取所有邮件
	DWORD load_all_mail(DWORD p_msg, DWORD dw_reserve);

	// 获取邮件内容
	DWORD load_mail_content(DWORD p_msg, DWORD dw_reserve);

	// 获取邮件物品
	DWORD load_mail_item(DWORD p_msg, DWORD dw_reserve);

	// 获取邮件最大编号
	DWORD get_mail_max_serial(DWORD p_msg, DWORD dw_reserve);

	// 更新邮件
	DWORD update_mail(DWORD p_msg, DWORD dw_reserve);

	// 删除邮件
	DWORD delete_mail(DWORD p_msg, DWORD dw_reserve);

	// 更新角色发邮件计数
	DWORD update_role_send_mail_count(DWORD p_msg, DWORD dw_reserve);

	// 更新挂机次数
	DWORD clean_role_hang_number(DWORD p_msg, DWORD dw_reserve);

	// 清空玩家配偶信息 gx add 2013.7.3
	DWORD clean_role_spouse_id(DWORD p_msg, DWORD dw_reserve);
	// 清空玩家VIP信息 gx add 2013.8.14
	DWORD clean_role_vip_info(DWORD p_msg,DWORD dw_reserve);
	
	DWORD clean_role_day_clear(DWORD p_msg, DWORD dw_reserve);

	DWORD reset_role_at_0(DWORD p_msg, DWORD dw_reserve);

	// 更新元宝兑换次数
	DWORD clean_role_exchange_num(DWORD p_msg,	DWORD dw_reserve);

	// 更新拍卖次数
	DWORD clean_role_paimai_number(DWORD p_msg, DWORD dw_reserver);

	// 更新签到数据
	DWORD clean_role_sign_data(DWORD p_msg, DWORD dw_reserver);

	// 更新钱庄拍卖次数
	DWORD clean_role_bank_number(DWORD p_msg, DWORD dw_reserver);

	// 更新战功商店购买次数
	DWORD clean_role_exploits_num(DWORD p_msg, DWORD dw_reserver);

	// 更新活跃度数据
	DWORD clean_role_active_data(DWORD p_msg, DWORD dw_reserver);

	// 更新1v1每日积分
	DWORD update_day_1v1_score(DWORD p_msg, DWORD dw_reserve);

	// 更新1v1周积分
	DWORD update_week_1v1_score(DWORD p_msg, DWORD dw_reserve);

	//更新1v1领奖标志
	DWORD update_1v1_award(DWORD p_msg, DWORD dw_reserve);
	
	// 更新幸运值
	DWORD clean_role_luck(DWORD p_msg, DWORD dw_reserve);

	// 更新噬魂
	DWORD update_shihun(DWORD p_msg, DWORD dw_reserve);

	// 清空摧毁装备次数
	DWORD clear_destory_equip_count(DWORD p_msg, DWORD dw_reserve);

	//// 更新物品冷却时间
	//DWORD ReplaceItemCDTime(DWORD p_msg, DWORD dw_reserve);

	// 改变人物金钱
	DWORD change_role_money(DWORD p_msg, DWORD dw_reserve);

	// 获取拍卖最大编号
	DWORD get_paimai_max_id(DWORD p_msg, DWORD dw_reserve);

	// 更新拍卖物品
	DWORD update_paimai_item(DWORD p_msg, DWORD dw_reserver);

	// 保存拍卖信息
	DWORD insert_paimai(DWORD p_msg, DWORD dw_reserver);

	// 读取所有拍卖数据
	DWORD load_all_paimai_info(DWORD p_msg, DWORD dw_reserver);

	// 读取所有自动拍卖数据
	DWORD load_all_auto_paimai(DWORD p_msg, DWORD dw_reserver);

	// 检测是否已经拍卖
	DWORD check_is_paimai(DWORD p_msg, DWORD dw_reserver);

	// 自动拍卖初始化完成
	DWORD auto_paimai_init_ok(DWORD p_msg, DWORD dw_reserver);

	// 更新自动拍卖
	DWORD update_auto_paimai(DWORD p_msg, DWORD dw_reserver);

	// 读取拍卖物品
	DWORD load_paimai_item(DWORD p_msg,	DWORD dw_reserver);

	// 删除拍卖
	DWORD delete_paimai(DWORD p_msg, DWORD dw_reserver);

	// 更新拍卖
	DWORD update_paimai(DWORD p_msg, DWORD dw_reserver);

	// 获取钱庄最大编号
	DWORD	get_bank_max_id(DWORD p_msg, DWORD dw_reserver);

	// 保存钱庄信息
	DWORD	insert_bank(DWORD p_msg, DWORD dw_reserver);

	// 删除钱庄信息
	DWORD	delete_bank(DWORD p_msg, DWORD dw_reserver);

	// 更新钱庄信息
	DWORD	update_bank(DWORD p_msg, DWORD dw_reserver);

	// 读取所有钱庄信息
	DWORD	load_all_bank(DWORD p_msg, DWORD dw_reserver);
private:
	// 读取等级排行榜
	DWORD	load_level_rank(DWORD p_msg, DWORD dw_reserve);

	// 读取装备排行榜
	DWORD	load_equip_rank(DWORD p_msg, DWORD dw_reserve);

	// 读取帮会排行榜
	DWORD	load_guild_rank(DWORD p_msg, DWORD dw_reserve);

	// 读取恶人榜
	DWORD	load_kill_rank(DWORD p_msg, DWORD dw_reserve);

	// 读取正义榜
	DWORD	load_justice_rank(DWORD p_msg, DWORD dw_reserve);

	// 读取噬魂榜
	DWORD	load_shihun_rank(DWORD p_msg, DWORD dw_reserve);

	// 读取1v1积分榜
	DWORD	load_1v1_rank(DWORD p_msg, DWORD dw_reserve);
	
	// 成就点数榜
	DWORD	load_achpoint_rank(DWORD p_msg, DWORD dw_reserve);
	
	// 成就数量榜
	DWORD	load_achnumber_rank(DWORD p_msg, DWORD dw_reserve);

	// 师徒榜 gx add 2013.12.06
	DWORD	load_master_rank(DWORD p_msg,DWORD dw_reserve);

	// 坐骑榜
	DWORD	load_mounts_rank(DWORD p_msg, DWORD dw_reserve);

	// 充值榜
	DWORD	load_reach_rank(DWORD p_msg, DWORD dw_reserve);
private: 
	// 好友
	DWORD insert_friend(DWORD p_msg, DWORD dw_reserve);
	DWORD update_friend(DWORD p_msg, DWORD dw_reserve);
	DWORD delelte_friend(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_all_friend_relation(DWORD p_msg, DWORD dw_reserve);

	// 友好度
	DWORD insert_friendship(DWORD p_msg, DWORD dw_reserve);
	DWORD update_friendship(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_friendship(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_all_friendship_relation(DWORD p_msg, DWORD dw_reserve);

	// 黑名单
	DWORD insert_black(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_black(DWORD p_msg, DWORD dw_reserve);

	// 敌人
	DWORD insert_enemy(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_enemy(DWORD p_msg, DWORD dw_reserve);

	// 技能
	DWORD insert_skill(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_skill(DWORD p_msg, DWORD dw_reserve);
	DWORD update_skill(DWORD p_msg, DWORD dw_reserve);
	
	// 物品(装备)
	DWORD insert_item(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_equip(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_item(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_equip(DWORD p_msg, DWORD dw_reserve);
	DWORD save_equip_special(DWORD p_msg, DWORD dw_reserve);

	// 百宝物品
	DWORD insert_baibao_item(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_baibao_equip(DWORD p_msg, DWORD dw_reserve);

	// 称号
	DWORD insert_title(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_title(DWORD p_msg, DWORD dw_reserve);

	// 角色名帖
	DWORD update_visiting_card(DWORD p_msg, DWORD dw_reserve);
	DWORD load_visiting_card(DWORD p_msg, DWORD dw_reserve);

	// 声望/名人堂/家族珍宝
	DWORD load_reputation_rank(DWORD p_msg, DWORD dw_reserve);
	DWORD fame_hall_initial(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_act_treasure(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_fame_hall_enter_snap(DWORD p_msg, DWORD dw_reserve);
	DWORD update_rep_reset_time(DWORD p_msg, DWORD dw_reserve);

	// 帮派属性
	DWORD load_all_guild(DWORD p_msg, DWORD dw_reserve);
	DWORD check_guild_init_ok(DWORD p_msg, DWORD dw_reserve);
	DWORD create_guild(DWORD p_msg, DWORD dw_reserve);
	DWORD dismiss_guild(DWORD p_msg, DWORD dw_reserve);
	DWORD resetSignUpAttack(DWORD p_msg, DWORD dw_reserve);
	DWORD change_guild_tenet(DWORD p_msg, DWORD dw_reserve);
	DWORD change_guild_symbol(DWORD p_msg, DWORD dw_reserve);
	DWORD save_guild_all(DWORD p_msg, DWORD dw_reserve);
	DWORD save_guild_all_plant(DWORD p_msg, DWORD dw_reserve);
	DWORD save_guild_attribute(DWORD p_msg, DWORD dw_reserve);
	DWORD change_guild_pos_name(DWORD p_msg, DWORD dw_reserve);
	DWORD update_guild_pos_power(DWORD p_msg, DWORD dw_reserve);
	DWORD load_guild_plant_data(DWORD p_msg, DWORD dw_reserve);
	DWORD save_guild_plant(DWORD p_msg, DWORD dw_reserve);
	DWORD load_sbk_guild(DWORD p_msg, DWORD dw_reserve);
	DWORD save_sbk_guild(DWORD p_msg, DWORD dw_reserve);

	// 帮派PVP
	DWORD insert_guild_pvp_data(DWORD p_msg, DWORD dw_reserve);
	DWORD load_guild_pvp_data(DWORD p_msg, DWORD dw_reserve);
	DWORD update_pvp_data(DWORD p_msg, DWORD dw_reserve);

	// 帮会招募榜
	DWORD load_guild_recruit(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_guild_recruit(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_recruit(DWORD p_msg, DWORD dw_reserve);
	DWORD clean_guild_recruit(DWORD p_msg,	DWORD dw_reserve);

	// 帮会boss击杀
	DWORD load_guild_skill_boss(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_guild_kill_boss(DWORD p_msg, DWORD dw_reserve);
	

	// 帮会战绩
	DWORD load_guild_war_history(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_guild_war_history(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_war_history(DWORD p_msg, DWORD dw_reserver);

	// 帮派成员
	DWORD load_all_guild_member(DWORD p_msg, DWORD dw_reserve);
	DWORD join_guild(DWORD p_msg, DWORD dw_reserve);
	DWORD leave_guild(DWORD p_msg, DWORD dw_reserve);
	DWORD change_guild_member_pos(DWORD p_msg, DWORD dw_reserve);
	DWORD guild_ware_privilege(DWORD p_msg, DWORD dw_reserve);
	DWORD change_member_contribution(DWORD p_msg, DWORD dw_reserve);
	DWORD change_ballot(DWORD p_msg, DWORD dw_reserve);
	//DWORD change_war(DWORD p_msg, DWORD dw_reserve);
	DWORD change_DKP(DWORD p_msg, DWORD dw_reserve);

	// 帮派仓库
	DWORD load_guild_ware_all_items(DWORD p_msg, DWORD dw_reserve);
	DWORD save_guild_ware_items(DWORD p_msg, DWORD dw_reserve);

	// 帮派设施
	DWORD load_guild_facilities(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_guild_facilities(DWORD p_msg, DWORD dw_reserve);
	DWORD update_guild_facilities(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_facilities(DWORD p_msg, DWORD dw_reserve);

	// 帮派弹劾
	DWORD insert_guild_delate(DWORD p_msg, DWORD dw_reserve);
	DWORD load_guild_delate(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_delate(DWORD p_msg, DWORD dw_reserve);
	DWORD update_guild_delate(DWORD p_msg, DWORD dw_reserve);
	DWORD update_guild_delate_content(DWORD p_msg, DWORD dw_reserve);

	// 帮派技能
	DWORD load_guild_skill_info(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_guild_skill_info(DWORD p_msg, DWORD dw_reserve);
	DWORD save_guild_skill_info(DWORD p_msg, DWORD dw_reserve);
	DWORD change_research_guild_skill(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_skill_info(DWORD p_msg, DWORD dw_reserve);

	// 帮派跑商
	DWORD load_guild_commerce_info(DWORD p_msg, DWORD dw_reserve);
	DWORD load_commerce_rank_info(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_commodity_info(DWORD p_msg, DWORD dw_reserve);
	DWORD update_commodity_tael(DWORD p_msg, DWORD dw_reserve);
	DWORD save_commodity_info(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_commodity_info(DWORD p_msg, DWORD dw_reserve);
	DWORD set_guild_commendation(DWORD p_msg, DWORD dw_reserve);
	DWORD update_commerce_rank_info(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_commerce_rank_info(DWORD p_msg, DWORD dw_reserve);

	// 宠物相关
	DWORD create_new_pet(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_pet(DWORD p_msg, DWORD dw_reserve);
	DWORD update_pet_attribute(DWORD p_msg, DWORD dw_reserve);

	DWORD insert_pet_skill(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_pet_skill(DWORD p_msg, DWORD dw_reserve);

	// 商城相关
	DWORD load_all_group_purchase_info(DWORD p_msg, DWORD dw_reserve);
	DWORD add_group_purchase_info(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_group_purchase_info(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_group_purchase_info(DWORD p_msg, DWORD dw_reserve);
	DWORD update_group_purchase_info(DWORD p_msg, DWORD dw_reserve);
	DWORD update_group_purchase_time(DWORD p_msg, DWORD dw_reserve);

	// VIP摊位
	DWORD get_all_vip_stall_info(DWORD p_msg, DWORD dw_reserve);
	DWORD apply_vip_stall(DWORD p_msg, DWORD dw_reserve);
	DWORD update_vip_stall_time(DWORD p_msg, DWORD dw_reserve);

	DWORD update_vip_net_bar_player(DWORD p_msg, DWORD dw_reserve);
	DWORD load_vip_net_bar(DWORD p_msg, DWORD dw_reserve);
	
	DWORD load_leave_msg(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_leave_msg(DWORD p_msg, DWORD dw_reserve);
	DWORD clear_leave_msg(DWORD p_msg, DWORD dw_reserve);

	// 更新宝箱数目
	DWORD update_treasure_count(DWORD p_msg, DWORD dw_reserve);

	//任务相关
	DWORD accept_quest(DWORD p_msg, DWORD dw_reserve);
	DWORD complete_quest(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_quest(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_all_dayquest(DWORD p_msg,DWORD dw_reserve);//gx add

	DWORD remove_all_teamshare_quest(DWORD p_msg, DWORD dw_reserve);

private: // 日志
	DWORD insert_money_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_yuanbao_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_exvolume_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_log_exploits(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_time_stat_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_item_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_mall_sell_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_mall_sell_pack_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_item_times_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_fund_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_material_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_reputation_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_gm_cmd_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_bank_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_paimai_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_leave_pracitice_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_serial_reward_log(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_kick_log(DWORD p_msg, DWORD dw_reserve);

	DWORD update_online_num(DWORD p_msg, DWORD dw_reserve);

private://师徒
	DWORD load_all_master_prentices( DWORD p_msg, DWORD dw_reserve );
	DWORD load_all_master_recruit( DWORD p_msg, DWORD dw_reserve );
	DWORD save_master_prentices(DWORD p_msg, DWORD dw_reserve);
	DWORD add_new_master_prentice(DWORD p_msg, DWORD dw_reserve );
	DWORD update_master_id_and_forbid_time(DWORD p_msg, DWORD dw_reserve );
	DWORD master_delete_role( DWORD p_msg, DWORD dw_reserve );
	DWORD join_master_recruit( DWORD p_msg, DWORD dw_reserve );
	DWORD leave_master_recruit( DWORD p_msg, DWORD dw_reserve );
private:
	// 重置玩家元气值
	DWORD reset_all_role_vigour(DWORD p_msg, DWORD dw_reserve);
private:
	//1v1积分竞技场
	DWORD update_role_1v1_score(DWORD p_msg,	DWORD dw_reserve);
	DWORD update_noline_1v1_score(DWORD p_msg, DWORD dw_reserve);

	// 坐骑
	DWORD updata_mounts(DWORD p_msg, DWORD dw_reserve);
private:
	// 悬赏任务
	DWORD load_all_guerdonquest(DWORD p_msg, DWORD dw_reserve);
	DWORD load_all_guerdonquest_reward(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_new_guerdonquest(DWORD p_msg, DWORD dw_reserve);
	DWORD update_guerdonquest(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guerdonquest(DWORD p_msg, DWORD dw_reserve);
	DWORD update_questcompletedata(DWORD p_msg, DWORD dw_reserve);
	DWORD update_questflag(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_quest_rewarditem(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_quest_rewarditem(DWORD p_msg, DWORD dw_reserve);

	// 宠物sns
	DWORD insert_pet_sns_info(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_pet_sns_info(DWORD p_msg, DWORD dw_reserve);
	DWORD load_pet_sns_info(DWORD p_msg, DWORD dw_reserve);
private:
		DWORD inc_circle_refreshnumber_and_questnum(DWORD p_msg, DWORD dw_reserve);
private:
	// 初始配置
	BOOL init_config();	
	// 初始游戏世界
	DWORD init_world(LPCTSTR sz_game_server);
	// 注册包处理
	VOID register_command();
	// 注销包处理
	VOID unregister_command();
	// 关闭游戏世界
	VOID terminate_world();
	// 创建log表
	VOID create_current_log_table();
private: 
	// 登陆回调
	DWORD callback_on_login(LPBYTE pByte, DWORD dw_size);
	// 下线回调
	DWORD callback_on_logout(DWORD dw_reserve);
private: 
	// 收包线程
	UINT recive_thread();
	static UINT WINAPI static_recive_thread(LPVOID p_data);

	// 逻辑处理线程
	UINT process_thread();
	static UINT WINAPI static_process_thread(LPVOID p_data);

	// 心跳线程
	UINT heartbeat_thead();
	static UINT WINAPI static_heartbeat_thead(LPVOID p_data);

private: 
	// 读取测试
	DWORD force_test_load_role(DWORD p_msg, DWORD dw_reserve);
	DWORD force_test_load_item(DWORD p_msg, DWORD dw_reserve);
	// 写入测试
	DWORD force_test_save_item(DWORD p_msg, DWORD dw_reserve);
	DWORD force_test_save_equip(DWORD p_msg, DWORD dw_reserve);
private:
	
	few_connect_server*	p_server_;				// 与游戏服务器的连接
	file_system*		p_vfs;

	// 同步锁
	db_mutex lock_;

	// 网络端口
	int	n_port_;	

	// DB服务器金色代码
	DWORD dw_db_gold_code_;	

	// 游戏服务器金色代码
	DWORD dw_game_gold_code_;	

	// 数据库操作
	db_handler* db_handler_;

	// 跳出循环
	volatile BOOL b_terminate_;	

	// 服务器关闭
	volatile BOOL b_server_disconnect_;

	// 是否有连接
	volatile BOOL b_has_server_connect_;

	// 游戏世界名称
	TCHAR sz_game_server_name_[X_SHORT_NAME];

	// 日志时间
	tagDWORDTime st_log_time_;	

	// 日志后缀
	char sz_log_file_postfix_[16];

	// 消息缓冲队列
	db_message_queue message_queue_;				

	// 角色缓冲(循环使用)
	char buffer_[X_ROLE_BUFF_SIZE];

	// 角色基本信息(循环使用)
	NET_DB2S_load_simrole* simple_role_;			

	// 角色完整信息(循环使用)
	NET_DB2S_load_role* full_role_;
	 
	// 创角信息(循环使用)
	NET_DB2S_create_role* create_role_;

	// 百宝信息(循环使用)
	NET_DB2S_load_baibao* load_baibao_;

	// 百宝日志(循环使用)
	NET_DB2S_load_baibao_log* load_baibao_log_;

private: 
	//数据库服务器统计
	struct status_record
	{
		// CPU内核数
		int n_cpu_number_;
		// 物理内存总量
		DWORD dw_total_phys_;
		// 可用物理内存
		DWORD dw_avail_phys_;
		// 非页面缓冲池使用
		int	n_quota_non_paged_pool_usage_;

		// 数据库版本
		DWORD dw_version_game_db_;
		// ODBC库版本
		DWORD dw_version_odbc_;
		// serverbase库版本
		DWORD dw_version_serverbase_;
		// serverdefine库版本
		DWORD dw_version_serverbasedefine_;
		// xmlreader库版本
		DWORD dw_version_xmlreader_;

		// 读数据库次数
		DWORD dw_read_count_;
		// 写数据库次数
		DWORD dw_write_count_;
		// 最后一次读数据库次数
		DWORD dw_last_read_count_;
		// 最后一次写数据库次数
		DWORD dw_last_write_count_;

		// 服务器心跳
		volatile DWORD dw_heartbeat_;			
		// 包总和
		volatile DWORD dw_recv_msg_count_;
		// 处理包总和
		volatile DWORD dw_process_msg_count_;
		// 发送包总和
		volatile DWORD dw_send_msg_count_;			
		// 接收包总大小
		DWORD dw_recv_msg_size_;
		// 发送包总大小
		DWORD dw_send_msg_size_;
		// 等待处理的包数
		volatile DWORD dw_wait_recv_msg_count_;
		// 等待发送的包数
		volatile DWORD dw_wait_send_msg_count_;
		// 最后一次接收包数
		DWORD dw_last_recv_msg_count_;
		// 最后一次处理包数
		DWORD dw_last_proc_msg_count_;
		// 最后一次发送包数
		DWORD dw_last_send_msg_count_;
		// 最后一次收包大小
		DWORD dw_last_recv_msg_size_;
		// 最后一次发包大小
		DWORD dw_last_send_msg_size_;

		// 一个tick内收包数
		DWORD dw_tick_recv_msg_count_;
		// 一个tick内处理包数
		DWORD dw_tick_proc_msg_count_;			
		// 一个tick内发包数
		DWORD dw_tick_send_msg_count_;
		// 一个tick内收包大小
		DWORD dw_tick_recv_msg_size_;
		// 一个tick内发包大小
		DWORD dw_tick_send_msg_size_;
		// 一个tick内读数据库次数
		DWORD dw_tick_read_db_count_;
		// 一个tick内写数据库次数
		DWORD dw_tick_write_db_count_;
		// 最近一个tick
		DWORD dw_last_tick_;
		// 频率(ms)
		DWORD dw_interval_;				
	public:
		status_record(){ init(); }
		
		VOID init()
		{
			ZeroMemory(this, sizeof(*this));
			n_cpu_number_ = get_tool()->get_cup_num();
			dw_last_tick_ = GetTickCount();
		}
	} status_record_;
private:
	// 取得状态
	const status_record* get_status_ptr( );
	status_record& get_status( ){ return status_record_; }
	const status_record& get_status( ) const { return status_record_; }
private: 
	// 创建窗口
	BOOL create_window(HINSTANCE hInst);
	// 刷新窗口
	VOID refresh_window();
};

#endif //__DATABASE_SERVER_HANDLER_H__
