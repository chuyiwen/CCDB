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
 *	@brief		���ݿ��������
*/


#ifndef __DATABASE_SERVER_HANDLER_H__

#define __DATABASE_SERVER_HANDLER_H__

#include "../common/ServerDefine/base_server_define.h"

//ǰ������
struct NET_DB2S_load_role;
struct NET_DB2S_load_simrole;
struct NET_DB2S_create_role;
struct tagNDBS_SaveRole;
struct NET_DB2S_load_baibao;
struct NET_DB2S_load_baibao_log;
struct NET_DB2S_get_all_gp_info;

class db_handler;

//���ݿ���Ϣ�������,�Ƚ��ȳ�����
//ֻ���洢���������ڴ���������ⲿ������Ҫ������Ӧ���ڴ����
class db_message_queue
{
public:
	db_message_queue()	
	{ 
		//�ֶ�����
		event_handle_ = ::CreateEvent(NULL, FALSE, TRUE, NULL);
	}

	~db_message_queue()	{ ::CloseHandle(event_handle_); }

public:
	// ��β�����һ����Ϣ
	VOID add_msg(LPBYTE p_msg)
	{
		msg_queue_.push_back(p_msg);
		
		// �����ڴ��¼��ϵȴ��������߳�
		::SetEvent(event_handle_);
	}

	// ȡ�õ�һ����Ϣ�����ⲿ��������Ҫ������Ӧ�ڴ�
	LPBYTE	get_msg(){ return msg_queue_.pop_front(); }

	// ��ȡhandle
	HANDLE	get_event() { return event_handle_; }

	// �������Ϣ����
	int		msg_count() { return msg_queue_.size(); }

private:
	// ��Ϣ����
	package_safe_list<LPBYTE>	msg_queue_;
	// ͬ���¼�
	HANDLE				event_handle_;
};

// ���ݿ������
class db_server
{
public:
	db_server();
	~db_server();

public:
	static file_container* s_p_var;
	static thread_manager*		p_thread;
	// ����
	BOOL start(HINSTANCE hInst);
	// ��ѭ��
	VOID main_loop();
	// �ر��߳�
	VOID shutdown() { Interlocked_Exchange((long*)(&b_terminate_), TRUE); }
public:
	// ��Ҫ�ر�
	BOOL is_terminate() const { return b_terminate_; }
	// �Ƿ�����
	BOOL is_connect() const { return !b_server_disconnect_ && b_has_server_connect_; }
	// ��ǰ����
	DWORD get_heartbeat() const { return get_status( ).dw_heartbeat_; }

	file_system* get_file_system() { return p_vfs; }
private: //������Ϣ����ӿ�

	// ��ȡ������һ�������
	DWORD load_all_role_simple(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ��ɫ������
	DWORD load_one_role_simple(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ��ɫ��������
	DWORD load_one_role_full_data(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ�˺����к����
	DWORD load_serial_reward(DWORD p_msg, DWORD dw_reserve);

	// �洢��ɫ
	VOID save_one_role(DWORD p_msg, DWORD dw_reserve);

	//gx add 2013.9.17 ��ʱ������ҽ�������
	VOID update_one_role_reward_data(DWORD p_msg,DWORD dw_reserve);

	// ������ɫ
	DWORD create_role(DWORD p_msg, DWORD dw_reserve);

	// ɾ����ɫ
	DWORD delete_role(DWORD p_msg, DWORD dw_reserve);

	// �ָ�ɾ����ɫ����ʱ��
	DWORD role_guard_cancel_time(DWORD p_msg, DWORD dw_reserve);

	// ����ɾ����ɫ����ʱ��
	DWORD role_guard_set_time(DWORD p_msg, DWORD dw_reserve);

	// ���¼���ɾ���Ľ�ɫ
	DWORD active_role(DWORD p_msg, DWORD dw_reserve);
	
	// ��ɫ����
	DWORD change_role_name(DWORD p_msg, DWORD dw_reserve);

	// ��ɫ���İ��
	DWORD change_role_guild(DWORD p_msg, DWORD dw_reserve);

	// ��ɫ�뿪���ʱ��
	DWORD update_role_leave_guild_time(DWORD p_msg, DWORD dw_reserve);

	// ���½�ɫVIP��Ϣ gx add 2013.8.14
	DWORD update_role_vip_info(DWORD p_msg,DWORD dw_reserver);
	
	// ��ɫ����ģ��ID
	DWORD change_role_dress_model_id(DWORD p_msg, DWORD dw_reserve);

	// ������ɾ����������
	DWORD delete_map_process(DWORD p_msg, DWORD dw_reserve);

	// ���½�������
	DWORD update_role_map_limit(DWORD p_msg, DWORD dw_reserve);

	// ɾ����������
	DWORD delete_role_map_limit(DWORD p_msg, DWORD dw_reserve);

	// ������������
	DWORD insert_role_map_limit(DWORD p_msg, DWORD dw_reserve);

	// ��Ϸ��������ʼ���ɹ�
	DWORD game_server_init_ok(DWORD p_msg, DWORD dw_reserve);

	// ��Ϸ�������ر�
	DWORD game_server_showdown(DWORD p_msg, DWORD dw_reserve);
	
	// ��GameServer���س�ʼ�����
	DWORD game_server_ok(DWORD p_msg, DWORD dw_reserve);

	// ������Ʒ����Ψһ���/��С����
	DWORD get_world_max_min_serial(DWORD p_msg, DWORD dw_reserve);	

	// ��ȡ�ٱ���Ʒ
	DWORD load_item_baibao(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ�ٱ�����Ԫ����Ϣ
	DWORD load_baibao_yuanbao(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ��ҳ������Ϣ
	DWORD load_web_receive(DWORD p_msg, DWORD dw_reserver);

	// ����Ԫ����ֵ
	DWORD update_yuanbao_recharge(DWORD p_msg, DWORD dw_reserve);

	// ����Ԫ����ֵ����
	DWORD update_total_recharge(DWORD p_msg, DWORD dw_reserve);

	// ɾ���ٱ���Ʒ
	DWORD delete_baibao_item(DWORD p_msg, DWORD dw_reserve);

	// ��ɾ���ٱ�װ��
	DWORD delete_baibao_equip(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ��log��item����
	DWORD load_item_need_log(DWORD p_msg, DWORD dw_reserve);

	// ��ȡԪ���˻�
	DWORD load_all_yuanbao_account(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ����Ԫ������
	DWORD load_all_yuanbao_order(DWORD p_msg, DWORD dw_reserve);

	// ����Ԫ���˻�
	DWORD insert_yuanbao_account(DWORD p_msg, DWORD dw_reserve);

	// �����˻�Ԫ��
	DWORD update_account_yuanbao(DWORD p_msg, DWORD dw_reserve);

	// �����˻���Ǯ
	DWORD update_account_silver(DWORD p_msg, DWORD dw_reserve);

	// ��������
	DWORD update_submit_sell(DWORD p_msg, DWORD dw_reserve);

	// �����չ�
	DWORD update_submit_buy(DWORD p_msg, DWORD dw_reserve);

	// ��������
	DWORD insert_one_order(DWORD p_msg, DWORD dw_reserve);

	// ��ɶ���
	DWORD complete_one_order(DWORD p_msg, DWORD dw_reserve);

	// ˢ�¶���
	DWORD refresh_one_order(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ��ɫԪ������
	DWORD load_one_role_all_yuanbao_order(DWORD p_msg, DWORD dw_reserve);

	// ɾ������
	DWORD delete_one_order(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ�ٱ���־
	DWORD load_baibao_log_info(DWORD p_msg, DWORD dw_reserve);

	// �����ٱ���־
	DWORD insert_baibao_log_info(DWORD p_msg, DWORD dw_reserve);

	// ���°����ǰ󶨱�
	DWORD bag_meony_update(DWORD p_msg, DWORD dw_reserve);
	
	// �ʺ�����
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

	// ��ȡ���л����
	DWORD load_activities(DWORD p_msg, DWORD dw_reserve);

	// �洢ĳ�������
	DWORD save_activity(DWORD p_msg, DWORD dw_reserve);

	// ���½�ɫ����
	DWORD update_role_attribute(DWORD p_msg, DWORD dw_reserve);

	// ���½�ɫ���Ե�
	DWORD update_role_attribute_point(DWORD p_msg, DWORD dw_reserve);

	// ���½�ɫ�츳��
	DWORD update_role_talent_point(DWORD p_msg, DWORD dw_reserve);

	// �����ɫ�����߾���
	DWORD clear_role_leave_exp(DWORD p_msg, DWORD dw_reserve);

	// �����ɫ����������ֵ
	DWORD clear_role_leave_brotherhood(DWORD p_msg, DWORD dw_reserve);

	// �����ʼ�
	DWORD save_mail(DWORD p_msg, DWORD dw_reserve);

	// �����ʼ���Ʒ
	DWORD update_mail_item(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ�����ʼ�
	DWORD load_all_mail(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ�ʼ�����
	DWORD load_mail_content(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ�ʼ���Ʒ
	DWORD load_mail_item(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ�ʼ������
	DWORD get_mail_max_serial(DWORD p_msg, DWORD dw_reserve);

	// �����ʼ�
	DWORD update_mail(DWORD p_msg, DWORD dw_reserve);

	// ɾ���ʼ�
	DWORD delete_mail(DWORD p_msg, DWORD dw_reserve);

	// ���½�ɫ���ʼ�����
	DWORD update_role_send_mail_count(DWORD p_msg, DWORD dw_reserve);

	// ���¹һ�����
	DWORD clean_role_hang_number(DWORD p_msg, DWORD dw_reserve);

	// ��������ż��Ϣ gx add 2013.7.3
	DWORD clean_role_spouse_id(DWORD p_msg, DWORD dw_reserve);
	// ������VIP��Ϣ gx add 2013.8.14
	DWORD clean_role_vip_info(DWORD p_msg,DWORD dw_reserve);
	
	DWORD clean_role_day_clear(DWORD p_msg, DWORD dw_reserve);

	DWORD reset_role_at_0(DWORD p_msg, DWORD dw_reserve);

	// ����Ԫ���һ�����
	DWORD clean_role_exchange_num(DWORD p_msg,	DWORD dw_reserve);

	// ������������
	DWORD clean_role_paimai_number(DWORD p_msg, DWORD dw_reserver);

	// ����ǩ������
	DWORD clean_role_sign_data(DWORD p_msg, DWORD dw_reserver);

	// ����Ǯׯ��������
	DWORD clean_role_bank_number(DWORD p_msg, DWORD dw_reserver);

	// ����ս���̵깺�����
	DWORD clean_role_exploits_num(DWORD p_msg, DWORD dw_reserver);

	// ���»�Ծ������
	DWORD clean_role_active_data(DWORD p_msg, DWORD dw_reserver);

	// ����1v1ÿ�ջ���
	DWORD update_day_1v1_score(DWORD p_msg, DWORD dw_reserve);

	// ����1v1�ܻ���
	DWORD update_week_1v1_score(DWORD p_msg, DWORD dw_reserve);

	//����1v1�콱��־
	DWORD update_1v1_award(DWORD p_msg, DWORD dw_reserve);
	
	// ��������ֵ
	DWORD clean_role_luck(DWORD p_msg, DWORD dw_reserve);

	// �����ɻ�
	DWORD update_shihun(DWORD p_msg, DWORD dw_reserve);

	// ��մݻ�װ������
	DWORD clear_destory_equip_count(DWORD p_msg, DWORD dw_reserve);

	//// ������Ʒ��ȴʱ��
	//DWORD ReplaceItemCDTime(DWORD p_msg, DWORD dw_reserve);

	// �ı������Ǯ
	DWORD change_role_money(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ���������
	DWORD get_paimai_max_id(DWORD p_msg, DWORD dw_reserve);

	// ����������Ʒ
	DWORD update_paimai_item(DWORD p_msg, DWORD dw_reserver);

	// ����������Ϣ
	DWORD insert_paimai(DWORD p_msg, DWORD dw_reserver);

	// ��ȡ������������
	DWORD load_all_paimai_info(DWORD p_msg, DWORD dw_reserver);

	// ��ȡ�����Զ���������
	DWORD load_all_auto_paimai(DWORD p_msg, DWORD dw_reserver);

	// ����Ƿ��Ѿ�����
	DWORD check_is_paimai(DWORD p_msg, DWORD dw_reserver);

	// �Զ�������ʼ�����
	DWORD auto_paimai_init_ok(DWORD p_msg, DWORD dw_reserver);

	// �����Զ�����
	DWORD update_auto_paimai(DWORD p_msg, DWORD dw_reserver);

	// ��ȡ������Ʒ
	DWORD load_paimai_item(DWORD p_msg,	DWORD dw_reserver);

	// ɾ������
	DWORD delete_paimai(DWORD p_msg, DWORD dw_reserver);

	// ��������
	DWORD update_paimai(DWORD p_msg, DWORD dw_reserver);

	// ��ȡǮׯ�����
	DWORD	get_bank_max_id(DWORD p_msg, DWORD dw_reserver);

	// ����Ǯׯ��Ϣ
	DWORD	insert_bank(DWORD p_msg, DWORD dw_reserver);

	// ɾ��Ǯׯ��Ϣ
	DWORD	delete_bank(DWORD p_msg, DWORD dw_reserver);

	// ����Ǯׯ��Ϣ
	DWORD	update_bank(DWORD p_msg, DWORD dw_reserver);

	// ��ȡ����Ǯׯ��Ϣ
	DWORD	load_all_bank(DWORD p_msg, DWORD dw_reserver);
private:
	// ��ȡ�ȼ����а�
	DWORD	load_level_rank(DWORD p_msg, DWORD dw_reserve);

	// ��ȡװ�����а�
	DWORD	load_equip_rank(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ������а�
	DWORD	load_guild_rank(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ���˰�
	DWORD	load_kill_rank(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ�����
	DWORD	load_justice_rank(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ�ɻ��
	DWORD	load_shihun_rank(DWORD p_msg, DWORD dw_reserve);

	// ��ȡ1v1���ְ�
	DWORD	load_1v1_rank(DWORD p_msg, DWORD dw_reserve);
	
	// �ɾ͵�����
	DWORD	load_achpoint_rank(DWORD p_msg, DWORD dw_reserve);
	
	// �ɾ�������
	DWORD	load_achnumber_rank(DWORD p_msg, DWORD dw_reserve);

	// ʦͽ�� gx add 2013.12.06
	DWORD	load_master_rank(DWORD p_msg,DWORD dw_reserve);

	// �����
	DWORD	load_mounts_rank(DWORD p_msg, DWORD dw_reserve);

	// ��ֵ��
	DWORD	load_reach_rank(DWORD p_msg, DWORD dw_reserve);
private: 
	// ����
	DWORD insert_friend(DWORD p_msg, DWORD dw_reserve);
	DWORD update_friend(DWORD p_msg, DWORD dw_reserve);
	DWORD delelte_friend(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_all_friend_relation(DWORD p_msg, DWORD dw_reserve);

	// �Ѻö�
	DWORD insert_friendship(DWORD p_msg, DWORD dw_reserve);
	DWORD update_friendship(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_friendship(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_all_friendship_relation(DWORD p_msg, DWORD dw_reserve);

	// ������
	DWORD insert_black(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_black(DWORD p_msg, DWORD dw_reserve);

	// ����
	DWORD insert_enemy(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_enemy(DWORD p_msg, DWORD dw_reserve);

	// ����
	DWORD insert_skill(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_skill(DWORD p_msg, DWORD dw_reserve);
	DWORD update_skill(DWORD p_msg, DWORD dw_reserve);
	
	// ��Ʒ(װ��)
	DWORD insert_item(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_equip(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_item(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_equip(DWORD p_msg, DWORD dw_reserve);
	DWORD save_equip_special(DWORD p_msg, DWORD dw_reserve);

	// �ٱ���Ʒ
	DWORD insert_baibao_item(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_baibao_equip(DWORD p_msg, DWORD dw_reserve);

	// �ƺ�
	DWORD insert_title(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_title(DWORD p_msg, DWORD dw_reserve);

	// ��ɫ����
	DWORD update_visiting_card(DWORD p_msg, DWORD dw_reserve);
	DWORD load_visiting_card(DWORD p_msg, DWORD dw_reserve);

	// ����/������/�����䱦
	DWORD load_reputation_rank(DWORD p_msg, DWORD dw_reserve);
	DWORD fame_hall_initial(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_act_treasure(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_fame_hall_enter_snap(DWORD p_msg, DWORD dw_reserve);
	DWORD update_rep_reset_time(DWORD p_msg, DWORD dw_reserve);

	// ��������
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

	// ����PVP
	DWORD insert_guild_pvp_data(DWORD p_msg, DWORD dw_reserve);
	DWORD load_guild_pvp_data(DWORD p_msg, DWORD dw_reserve);
	DWORD update_pvp_data(DWORD p_msg, DWORD dw_reserve);

	// �����ļ��
	DWORD load_guild_recruit(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_guild_recruit(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_recruit(DWORD p_msg, DWORD dw_reserve);
	DWORD clean_guild_recruit(DWORD p_msg,	DWORD dw_reserve);

	// ���boss��ɱ
	DWORD load_guild_skill_boss(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_guild_kill_boss(DWORD p_msg, DWORD dw_reserve);
	

	// ���ս��
	DWORD load_guild_war_history(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_guild_war_history(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_war_history(DWORD p_msg, DWORD dw_reserver);

	// ���ɳ�Ա
	DWORD load_all_guild_member(DWORD p_msg, DWORD dw_reserve);
	DWORD join_guild(DWORD p_msg, DWORD dw_reserve);
	DWORD leave_guild(DWORD p_msg, DWORD dw_reserve);
	DWORD change_guild_member_pos(DWORD p_msg, DWORD dw_reserve);
	DWORD guild_ware_privilege(DWORD p_msg, DWORD dw_reserve);
	DWORD change_member_contribution(DWORD p_msg, DWORD dw_reserve);
	DWORD change_ballot(DWORD p_msg, DWORD dw_reserve);
	//DWORD change_war(DWORD p_msg, DWORD dw_reserve);
	DWORD change_DKP(DWORD p_msg, DWORD dw_reserve);

	// ���ɲֿ�
	DWORD load_guild_ware_all_items(DWORD p_msg, DWORD dw_reserve);
	DWORD save_guild_ware_items(DWORD p_msg, DWORD dw_reserve);

	// ������ʩ
	DWORD load_guild_facilities(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_guild_facilities(DWORD p_msg, DWORD dw_reserve);
	DWORD update_guild_facilities(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_facilities(DWORD p_msg, DWORD dw_reserve);

	// ���ɵ���
	DWORD insert_guild_delate(DWORD p_msg, DWORD dw_reserve);
	DWORD load_guild_delate(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_delate(DWORD p_msg, DWORD dw_reserve);
	DWORD update_guild_delate(DWORD p_msg, DWORD dw_reserve);
	DWORD update_guild_delate_content(DWORD p_msg, DWORD dw_reserve);

	// ���ɼ���
	DWORD load_guild_skill_info(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_guild_skill_info(DWORD p_msg, DWORD dw_reserve);
	DWORD save_guild_skill_info(DWORD p_msg, DWORD dw_reserve);
	DWORD change_research_guild_skill(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_skill_info(DWORD p_msg, DWORD dw_reserve);

	// ��������
	DWORD load_guild_commerce_info(DWORD p_msg, DWORD dw_reserve);
	DWORD load_commerce_rank_info(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_commodity_info(DWORD p_msg, DWORD dw_reserve);
	DWORD update_commodity_tael(DWORD p_msg, DWORD dw_reserve);
	DWORD save_commodity_info(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_commodity_info(DWORD p_msg, DWORD dw_reserve);
	DWORD set_guild_commendation(DWORD p_msg, DWORD dw_reserve);
	DWORD update_commerce_rank_info(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_commerce_rank_info(DWORD p_msg, DWORD dw_reserve);

	// �������
	DWORD create_new_pet(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_pet(DWORD p_msg, DWORD dw_reserve);
	DWORD update_pet_attribute(DWORD p_msg, DWORD dw_reserve);

	DWORD insert_pet_skill(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_pet_skill(DWORD p_msg, DWORD dw_reserve);

	// �̳����
	DWORD load_all_group_purchase_info(DWORD p_msg, DWORD dw_reserve);
	DWORD add_group_purchase_info(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_group_purchase_info(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guild_group_purchase_info(DWORD p_msg, DWORD dw_reserve);
	DWORD update_group_purchase_info(DWORD p_msg, DWORD dw_reserve);
	DWORD update_group_purchase_time(DWORD p_msg, DWORD dw_reserve);

	// VIP̯λ
	DWORD get_all_vip_stall_info(DWORD p_msg, DWORD dw_reserve);
	DWORD apply_vip_stall(DWORD p_msg, DWORD dw_reserve);
	DWORD update_vip_stall_time(DWORD p_msg, DWORD dw_reserve);

	DWORD update_vip_net_bar_player(DWORD p_msg, DWORD dw_reserve);
	DWORD load_vip_net_bar(DWORD p_msg, DWORD dw_reserve);
	
	DWORD load_leave_msg(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_leave_msg(DWORD p_msg, DWORD dw_reserve);
	DWORD clear_leave_msg(DWORD p_msg, DWORD dw_reserve);

	// ���±�����Ŀ
	DWORD update_treasure_count(DWORD p_msg, DWORD dw_reserve);

	//�������
	DWORD accept_quest(DWORD p_msg, DWORD dw_reserve);
	DWORD complete_quest(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_quest(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_all_dayquest(DWORD p_msg,DWORD dw_reserve);//gx add

	DWORD remove_all_teamshare_quest(DWORD p_msg, DWORD dw_reserve);

private: // ��־
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

private://ʦͽ
	DWORD load_all_master_prentices( DWORD p_msg, DWORD dw_reserve );
	DWORD load_all_master_recruit( DWORD p_msg, DWORD dw_reserve );
	DWORD save_master_prentices(DWORD p_msg, DWORD dw_reserve);
	DWORD add_new_master_prentice(DWORD p_msg, DWORD dw_reserve );
	DWORD update_master_id_and_forbid_time(DWORD p_msg, DWORD dw_reserve );
	DWORD master_delete_role( DWORD p_msg, DWORD dw_reserve );
	DWORD join_master_recruit( DWORD p_msg, DWORD dw_reserve );
	DWORD leave_master_recruit( DWORD p_msg, DWORD dw_reserve );
private:
	// �������Ԫ��ֵ
	DWORD reset_all_role_vigour(DWORD p_msg, DWORD dw_reserve);
private:
	//1v1���־�����
	DWORD update_role_1v1_score(DWORD p_msg,	DWORD dw_reserve);
	DWORD update_noline_1v1_score(DWORD p_msg, DWORD dw_reserve);

	// ����
	DWORD updata_mounts(DWORD p_msg, DWORD dw_reserve);
private:
	// ��������
	DWORD load_all_guerdonquest(DWORD p_msg, DWORD dw_reserve);
	DWORD load_all_guerdonquest_reward(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_new_guerdonquest(DWORD p_msg, DWORD dw_reserve);
	DWORD update_guerdonquest(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_guerdonquest(DWORD p_msg, DWORD dw_reserve);
	DWORD update_questcompletedata(DWORD p_msg, DWORD dw_reserve);
	DWORD update_questflag(DWORD p_msg, DWORD dw_reserve);
	DWORD insert_quest_rewarditem(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_quest_rewarditem(DWORD p_msg, DWORD dw_reserve);

	// ����sns
	DWORD insert_pet_sns_info(DWORD p_msg, DWORD dw_reserve);
	DWORD delete_pet_sns_info(DWORD p_msg, DWORD dw_reserve);
	DWORD load_pet_sns_info(DWORD p_msg, DWORD dw_reserve);
private:
		DWORD inc_circle_refreshnumber_and_questnum(DWORD p_msg, DWORD dw_reserve);
private:
	// ��ʼ����
	BOOL init_config();	
	// ��ʼ��Ϸ����
	DWORD init_world(LPCTSTR sz_game_server);
	// ע�������
	VOID register_command();
	// ע��������
	VOID unregister_command();
	// �ر���Ϸ����
	VOID terminate_world();
	// ����log��
	VOID create_current_log_table();
private: 
	// ��½�ص�
	DWORD callback_on_login(LPBYTE pByte, DWORD dw_size);
	// ���߻ص�
	DWORD callback_on_logout(DWORD dw_reserve);
private: 
	// �հ��߳�
	UINT recive_thread();
	static UINT WINAPI static_recive_thread(LPVOID p_data);

	// �߼������߳�
	UINT process_thread();
	static UINT WINAPI static_process_thread(LPVOID p_data);

	// �����߳�
	UINT heartbeat_thead();
	static UINT WINAPI static_heartbeat_thead(LPVOID p_data);

private: 
	// ��ȡ����
	DWORD force_test_load_role(DWORD p_msg, DWORD dw_reserve);
	DWORD force_test_load_item(DWORD p_msg, DWORD dw_reserve);
	// д�����
	DWORD force_test_save_item(DWORD p_msg, DWORD dw_reserve);
	DWORD force_test_save_equip(DWORD p_msg, DWORD dw_reserve);
private:
	
	few_connect_server*	p_server_;				// ����Ϸ������������
	file_system*		p_vfs;

	// ͬ����
	db_mutex lock_;

	// ����˿�
	int	n_port_;	

	// DB��������ɫ����
	DWORD dw_db_gold_code_;	

	// ��Ϸ��������ɫ����
	DWORD dw_game_gold_code_;	

	// ���ݿ����
	db_handler* db_handler_;

	// ����ѭ��
	volatile BOOL b_terminate_;	

	// �������ر�
	volatile BOOL b_server_disconnect_;

	// �Ƿ�������
	volatile BOOL b_has_server_connect_;

	// ��Ϸ��������
	TCHAR sz_game_server_name_[X_SHORT_NAME];

	// ��־ʱ��
	tagDWORDTime st_log_time_;	

	// ��־��׺
	char sz_log_file_postfix_[16];

	// ��Ϣ�������
	db_message_queue message_queue_;				

	// ��ɫ����(ѭ��ʹ��)
	char buffer_[X_ROLE_BUFF_SIZE];

	// ��ɫ������Ϣ(ѭ��ʹ��)
	NET_DB2S_load_simrole* simple_role_;			

	// ��ɫ������Ϣ(ѭ��ʹ��)
	NET_DB2S_load_role* full_role_;
	 
	// ������Ϣ(ѭ��ʹ��)
	NET_DB2S_create_role* create_role_;

	// �ٱ���Ϣ(ѭ��ʹ��)
	NET_DB2S_load_baibao* load_baibao_;

	// �ٱ���־(ѭ��ʹ��)
	NET_DB2S_load_baibao_log* load_baibao_log_;

private: 
	//���ݿ������ͳ��
	struct status_record
	{
		// CPU�ں���
		int n_cpu_number_;
		// �����ڴ�����
		DWORD dw_total_phys_;
		// ���������ڴ�
		DWORD dw_avail_phys_;
		// ��ҳ�滺���ʹ��
		int	n_quota_non_paged_pool_usage_;

		// ���ݿ�汾
		DWORD dw_version_game_db_;
		// ODBC��汾
		DWORD dw_version_odbc_;
		// serverbase��汾
		DWORD dw_version_serverbase_;
		// serverdefine��汾
		DWORD dw_version_serverbasedefine_;
		// xmlreader��汾
		DWORD dw_version_xmlreader_;

		// �����ݿ����
		DWORD dw_read_count_;
		// д���ݿ����
		DWORD dw_write_count_;
		// ���һ�ζ����ݿ����
		DWORD dw_last_read_count_;
		// ���һ��д���ݿ����
		DWORD dw_last_write_count_;

		// ����������
		volatile DWORD dw_heartbeat_;			
		// ���ܺ�
		volatile DWORD dw_recv_msg_count_;
		// ������ܺ�
		volatile DWORD dw_process_msg_count_;
		// ���Ͱ��ܺ�
		volatile DWORD dw_send_msg_count_;			
		// ���հ��ܴ�С
		DWORD dw_recv_msg_size_;
		// ���Ͱ��ܴ�С
		DWORD dw_send_msg_size_;
		// �ȴ�����İ���
		volatile DWORD dw_wait_recv_msg_count_;
		// �ȴ����͵İ���
		volatile DWORD dw_wait_send_msg_count_;
		// ���һ�ν��հ���
		DWORD dw_last_recv_msg_count_;
		// ���һ�δ������
		DWORD dw_last_proc_msg_count_;
		// ���һ�η��Ͱ���
		DWORD dw_last_send_msg_count_;
		// ���һ���հ���С
		DWORD dw_last_recv_msg_size_;
		// ���һ�η�����С
		DWORD dw_last_send_msg_size_;

		// һ��tick���հ���
		DWORD dw_tick_recv_msg_count_;
		// һ��tick�ڴ������
		DWORD dw_tick_proc_msg_count_;			
		// һ��tick�ڷ�����
		DWORD dw_tick_send_msg_count_;
		// һ��tick���հ���С
		DWORD dw_tick_recv_msg_size_;
		// һ��tick�ڷ�����С
		DWORD dw_tick_send_msg_size_;
		// һ��tick�ڶ����ݿ����
		DWORD dw_tick_read_db_count_;
		// һ��tick��д���ݿ����
		DWORD dw_tick_write_db_count_;
		// ���һ��tick
		DWORD dw_last_tick_;
		// Ƶ��(ms)
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
	// ȡ��״̬
	const status_record* get_status_ptr( );
	status_record& get_status( ){ return status_record_; }
	const status_record& get_status( ) const { return status_record_; }
private: 
	// ��������
	BOOL create_window(HINSTANCE hInst);
	// ˢ�´���
	VOID refresh_window();
};

#endif //__DATABASE_SERVER_HANDLER_H__
