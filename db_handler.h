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

// ����
struct s_role_data_load;
struct s_role_data_save;
struct s_create_role_info;
struct s_skill_save;
struct s_enter_map_limit;
struct NET_DB2C_change_role_money;
struct NET_DB2C_update_role_send_num;

// ���ߡ�װ��
struct tagItem;
struct tagEquip;
struct s_equip_spec_update;
struct tagEquipSpec;
struct NET_DB2S_load_equip_rank;
// ����
struct s_db_pet_soul_create;
struct s_db_pet_skill;
struct s_db_pet_att;
struct tagPetSNSInfo;
// �Ź�
struct s_gp_info_key;
struct s_gp_info;
// ��̯
struct s_vip_stall_to_db;
//VIP����
struct s_db_vnb_players;
// ���
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
// �ʼ�
struct NET_DB2C_save_mail;
struct NET_DB2C_update_mail;
struct NET_DB2C_delete_mail;
// ����
struct NET_S2DB_insert_paimai;
struct NET_S2DB_delete_paimai;
struct NET_S2DB_update_paimai;
struct NET_S2DB_update_auto_paimai;

// Ǯׯ
struct NET_S2DB_insert_bank;
struct NET_S2DB_delete_bank;
struct NET_S2DB_update_bank;

struct s_guild_plant;
//--------------------------------------------------------------------
// ���ݿ⴦���ִ࣬�в�ѯ�����¡�ɾ�����滻(replace into)����
// �ṩһ��ͨ�õķ���: �н��������: common_select (��ѯ)
//					   �޽��������: common_update (���¡�ɾ�����滻)
//!!!!!				   �޽�����첽��־����: common_update_log(���¡�ɾ�����滻)					  
// 1.����Ҫ���ڴ�ȫ���ⲿ�����߷��䲢��������ֻʹ�ò�����
// 2.������ݿⷵ�صĽ�������ɹ��첻�������������ݰ�
// 3.�����ݿ�д����߸��¶�������ʱ��ÿ�����ݵĴ�С����һ��
// 4.����common_updateֻ���ṩһ����ʽ����Ӧ�ֶ�����func_param3_1�ĺ��� 
// 5.����common_select���˵��������躯������Ҫ�ṩһ��������������func_result_proc�ĺ���
// 6.��־ʵʱ�Բ���,�������첽��ʽ��Ҫ����func_param3_2�ĺ���
//--------------------------------------------------------------------

// ���ݿ⴦����
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
	// ��������
	BOOL start(db_server* p_db_server, LPCTSTR sz_init_file, few_connect_server* p_server);
	
	// ���ٺ���
	VOID destroy();
public: // �˺�ϵͳ
	// �˺Ź���
	BOOL load_one_account_common(LPVOID p_buffer, DWORD dw_account_id);

	// ��ӹ���
	BOOL insert_account_common(DWORD dw_account_id, const char* sz_account);

	// ���°�ȫ��
	BOOL update_safe_code(DWORD dw_account_id, DWORD dw_safe_code_crc);

	// ���ð�ȫ������ʱ��
	BOOL update_safe_code_reset_time(DWORD dw_account_id, DWORD dw_reset_time);

	// ���±�������
	BOOL update_bag_password(DWORD dw_account_id, DWORD dw_bag_password);

	// ���½�ɫ�ֿ�����
	BOOL update_ware_size(DWORD dw_account_id, LPVOID p_buffer);

	// ���²ֿ����
	BOOL update_ware_money(DWORD dw_account_id, int64 n64_ware_silver);

	// ���°����ǰ󶨱�
	BOOL update_bag_money(DWORD	dw_role_id,	LPVOID p_buffer);

	// ����Ԫ��
	BOOL update_baibao_yuanbao(DWORD dw_account_id, LPVOID p_buffer);

	// ���»���
	BOOL update_exchang_volume(DWORD dw_account_id, LPVOID p_buffer);

	// ������ȡ��־
	BOOL update_receive(DWORD dw_account_id, LPVOID p_buffer);

	// ������ȡ��־
	BOOL update_receive_ex(DWORD dw_account_id, LPVOID p_buffer);

	// ������ҳ�콱��־
	BOOL update_web_receive(DWORD dw_account_id, LPVOID p_buffer);

	// ���³�ֵ����
	BOOL update_total_recharge(DWORD dw_account_id, LPVOID p_buffer);

	// ɾ�����к����
	BOOL delete_serial_reward(DWORD dw_account_id, LPVOID p_buffer);

	// ����
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

public:// Ԫ���˻�
	// ��ȡԪ���˻�
	BOOL load_all_yuanbao_account(LPVOID p_buffer, int &n_count);

	// ��ȡԪ������
	BOOL load_all_yuanbao_order(LPVOID p_buffer, int &n_count);

	// ��ȡ��ɫԪ������
	BOOL load_one_role_all_yuanbao_order(LPVOID p_buffer, int&n_count, DWORD dw_role_id);

	// ����Ԫ���˻�
	BOOL insert_yuanbao_account(DWORD dw_role_id);

	// �����˻�Ԫ��
	BOOL update_account_yuanbao(DWORD dw_role_id, int n_yuanbao);

	// �����˻���Ǯ
	BOOL update_account_silver(DWORD dw_role_id, LPVOID p_money);

	// ��������
	BOOL update_submit_sell(DWORD dw_role_id, bool b_sell);

	// �����չ�
	BOOL update_submit_buy(DWORD dw_role_id, bool b_buy);

	// ��������
	BOOL insert_one_order(DWORD dw_role_id, LPVOID p_order);

	// ��ɶ���
	BOOL complete_one_order(DWORD dw_role_id, LPVOID p_order);

	// ˢ�¶���
	BOOL refresh_one_order(DWORD dw_role_id, LPVOID p_order);

	// ɾ������
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
	// ������ɫ
	BOOL create_role(OUT DWORD &dw_new_role_id, DWORD dw_account_id, const s_create_role_info &role_info, DWORD &dw_role_name_crc);

	// ���ݽ�ɫ
	DWORD backup_role(DWORD dw_account_id, DWORD dw_role_id);

	// ɾ����ɫ
	DWORD delete_role(DWORD dw_account_id, DWORD dw_role_id);

	// ��¼ɾ����ɫ
	BOOL register_deleted_role(DWORD dw_account_id, DWORD dw_role_id, LPCTSTR sz_role_name, LPCSTR sz_ip);

	// ����ɾ����ɫ
	BOOL active_deleted_role(DWORD dw_role_id);

	// ��ȡ������һ�������
	BOOL load_all_role_simple(LPVOID p_buffer, int &n_role_count);

	// ��ȡ��ɫ������
	BOOL load_one_role_simple(DWORD dw_account_id, int &n_role_count, LPVOID p_sample_role);

	// ��ȡ��ɫ��������
	BOOL load_one_role_full_data(DWORD dw_account_id, DWORD dw_role_id, s_role_data_load *p_role_data);

	// �洢��ɫ
	VOID save_one_role(DWORD dw_role_id, s_role_data_save *p_role_data);


	// ��ȡ���к����
	BOOL load_serial_reward(LPVOID p_buffer, DWORD dw_account_id);
private:
	// ���ݽ�ɫ
	BOOL do_backup_role(DWORD dw_account_id, DWORD dw_role_id);
	// ���ݵ���
	BOOL do_backup_item(DWORD dw_role_id);
	// ɾ����ɫ
	BOOL do_delete_role(DWORD dw_account_id, DWORD dw_role_id);

private://��ȡ�������
	// ��ȡ��ɫ��������
	BOOL load_one_role_data(LPVOID &p_buffer, DWORD dw_account_id, DWORD dw_role_id);
	
	// ��ȡ��ɫ����
	BOOL load_one_role_skill(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ��ɫbuff
	BOOL load_one_role_buff(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ��ɫ�ƺ�
	BOOL load_one_role_title(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ��ɫ����
	BOOL load_one_role_items(LPVOID &p_buffer, int &n_count, DWORD dw_account_id, DWORD dw_role_id);

	// ��ȡ��ɫ����
	BOOL load_one_role_friends(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ��ɫ����
	BOOL load_one_role_enemies(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ��ɫδ�������
	BOOL load_one_role_quest(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ��ɫ�������
	BOOL load_one_role_quest_done(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ��������������
	BOOL load_one_role_simple_quest_done(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ����CDʱ��
	BOOL load_one_role_item_cd(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ��ɫ�Ѻö�
	BOOL load_one_role_friendship(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ�ҵ��Ѻö�
	BOOL load_one_role_friendship_ex(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ������
	BOOL load_one_role_black(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ��ͼ����
	BOOL load_one_role_map_limit(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ��ɳɾ�
	BOOL load_complate_achievement(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ�ɾͽ���
	BOOL load_progress_achievement(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡ��������
	BOOL load_process_instance(LPVOID &p_buffer, int &n_count, DWORD dw_role_id);

	// ��ȡǩ������
	BOOL load_sign_data(LPVOID &p_buffer, DWORD dw_role_id);
	
	// ��ȡ�꾧����
	BOOL load_huenjing_data(LPVOID &p_buffer, DWORD dw_role_id);
	
	// ��ȡ��������
	BOOL load_reward_data(LPVOID &p_buffer, DWORD dw_role_id);
	
	// ��������
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
public:// �޸���������
	// ��ɫ����
	BOOL change_role_name(DWORD dw_account_id, DWORD dw_role_id, LPCTSTR sz_new_role_name, DWORD dw_name_crc, DWORD dw_time);

	// �ָ�ɾ����ɫ����ʱ��
	BOOL role_guard_cancel_time(DWORD dw_account_id, DWORD dw_role_id);

	// ����ɾ����ɫ����ʱ��
	BOOL role_guard_set_time(DWORD dw_account_id, DWORD dw_role_id, DWORD dw_time);

	// ��ɫ���İ��
	BOOL change_role_guild(DWORD dw_role_id, DWORD dw_guild_id);

	// ��ɫ�뿪���ʱ��
	BOOL update_role_leave_guild_time(DWORD dw_role_id, LPVOID p_buffer);

	// ���½�ɫVIp��Ϣ
	BOOL update_role_vip_info(DWORD dw_role_id,LPVOID p_buffer);

	// ���ý�ɫɾ����־
	BOOL set_role_delete_flag(DWORD dw_role_id, DWORD dw_time);

	// ��ɫ����ģ��ID
	BOOL change_role_dress_model_id(DWORD dw_role_id, WORD w_value);

	// ���½�������
	BOOL update_role_map_limit(DWORD dw_role_id, int n_type);

	// ɾ����������
	BOOL delete_role_map_limit(DWORD dw_role_id);

	// ������������
	BOOL insert_role_map_limit(DWORD dw_role_id, s_enter_map_limit* p_map_limit);

	// ���½�ɫ����
	BOOL update_role_attribute(DWORD dw_role_id, PVOID p_buffer);

	// ���½�ɫ���Ե�
	BOOL update_role_attribute_point(DWORD dw_role_id, LPVOID p_msg);

	// ���½�ɫ�츳��
	BOOL update_role_talent_point(DWORD dw_role_id, LPVOID p_msg);

	// ��ս�ɫ�����߾���
	BOOL clear_role_leave_exp(DWORD dw_role_id, LPVOID p_msg);

	// ��ս�ɫ����������ֵ
	BOOL clear_role_leave_brotherhood(DWORD dw_role_id, LPVOID p_msg);

	// �����ż��Ϣ
	BOOL clear_role_spouse_id(DWORD dw_role_id, LPVOID p_msg);
	// ���VIP��Ϣ
	BOOL clear_role_vip_info(DWORD dw_role_id,LPVOID p_msg);

	// ��ս�ɫ�һ�����
	BOOL clear_role_hang_count(DWORD dw_role_id, LPVOID p_msg);
	
	BOOL clear_role_day_clear(DWORD dw_role_id, LPVOID p_msg);
	BOOL reset_role_at_0(DWORD dw_role_id, LPVOID p_msg);
	// ��ս�ɫǩ������
	BOOL clean_role_sign(DWORD dw_role_id, LPVOID p_msg);

	// ��ս�ɫԪ���һ�����
	BOOL clear_role_yuanbao_exchange_num(DWORD dw_role_id, LPVOID p_msg);
	
	// ��ս�ɫ��������
	BOOL clear_role_paimai_count(DWORD dw_role_id, LPVOID p_msg);

	// ��ս�ɫǮׯ��������
	BOOL clear_role_bank_count(DWORD dw_role_id, LPVOID p_msg);

	// ��ս�ɫս���̵깺�����
	BOOL clean_role_exploits_num(DWORD dw_role_id, LPVOID p_msg);

	// ��ս�ɫ��Ծ������
	BOOL clean_role_active_data(DWORD dw_role_id, LPVOID p_msg);

	// ����1v1ÿ�ջ���
	BOOL update_day_1v1_score(DWORD dw_role_id, LPVOID p_msg);

	// ����1v1�ܻ���
	BOOL update_week_1v1_score(DWORD dw_role_id, LPVOID p_msg);
	BOOL reset_role_1v1_score(DWORD dw_role_id, LPVOID p_msg);

	// ����1v1�콱��־
	BOOL update_1v1_award(DWORD dw_role_id, LPVOID p_msg);

	// ɾ����������
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
	// �����ɫ����
	BOOL save_role_data(DWORD dw_role_id, const s_role_data_save *p_role_data);

	// ���������Ϣ
	BOOL save_role_clan_data(DWORD dw_role_id, IN LPVOID p_buffer, BOOL bChange, OUT LPVOID* pp_buffer);

	// �����ɫ����
	BOOL save_role_skill(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// �����ɫBUFF
	BOOL save_role_buff(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// �����ɫ�ƺ�
	BOOL save_role_title(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// ��������ͼ����
	BOOL save_enter_map_limit(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// ��ɳɾ�
	BOOL save_complate_achievement(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// �ɾͽ���
	BOOL save_process_achievement(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// ��������
	BOOL save_process_instance(DWORD	dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);
	// ɾ����������
	BOOL delete_inst_process(DWORD	dw_role_id);

	// ǩ������
	BOOL save_sign_data(DWORD	dw_role_id, LPVOID p_buffer, OUT LPVOID *pp_buffer);
	
	BOOL save_huenjing_data(DWORD	dw_role_id, LPVOID p_buffer, OUT LPVOID *pp_buffer);

public:	BOOL save_reward_data(DWORD dw_role_id, LPVOID p_buffer, OUT LPVOID *pp_buffer);//������Ϊpublic gx modify 2013.9.17

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
	// ɾ��buff
	BOOL delete_role_buffs(DWORD dw_role_id);
	// ɾ����Ʒ
	BOOL delete_role_items(DWORD dw_role_id);

public:// ��Ʒװ�������

	// ������Ʒ
	BOOL insert_item(const tagItem &item, const char* sz_table = "item");

	// ����װ��
	BOOL insert_equip(const tagEquip &equip, BOOL b_baibao = FALSE);

	// ����װ������
	BOOL insert_equip_special(const tagEquip &equip, const char *sz_table = "equip");

	// ������Ʒ
	BOOL save_item(const tagItem &item);

	// ����װ������
	BOOL save_item_attribute(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// ����װ��
	BOOL save_equip(const tagEquip &equip);

	// ����װ������
	BOOL save_equip_special(const tagEquip &equip);
	BOOL save_equip_special(const s_equip_spec_update &equip_special_update);

	// ɾ����Ʒ
	BOOL delete_item(int64 n64_serial, const char *sz_table = "item");

	// ɾ��װ��
	BOOL delete_equip(int64 n64_serial);

	// ɾ��װ������
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

public: //����ɾ���ĵ��ߺ�װ��
	BOOL backup_deleted_equip(int64 n64_serial);
	BOOL backup_deleted_item(int64 n64_serial);

public:// ���µ���CDʱ��
	BOOL replace_item_cd_time(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);


public:// ��ɫ����
	
	// ��������
	BOOL insert_skill(DWORD dw_role_id, s_skill_save *p_buffer);

	// ɾ������
	BOOL delete_skill(DWORD dw_role_id, DWORD dw_skill_id);

	//���¼���
	BOOL update_skill(DWORD dw_role_id, s_skill_save *p_buffer);
private:
	VOID makesql_insert_skill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_skill(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);

public:// ����

	// �¼Ӻ���
	BOOL insert_friend(DWORD dw_role_id, LPVOID p_friend_save);

	// ���º���
	BOOL update_friend(DWORD dw_role_id, LPVOID p_friend_save);

	// ɾ������
	BOOL delelte_friend(DWORD dw_role_id, DWORD dw_friend_id);

	// ɾ����ɫ�ں��ѱ������м�¼
	BOOL delete_all_friend_relation(DWORD dw_role_id);
private: 
	VOID makesql_insert_friend(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_save);
	VOID makesql_update_friend(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_save);
	VOID makesqll_delete_friend(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_id);
	VOID makesql_delete_all_friend_relation(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);

public:// �Ѻö�
	
	// �������Ѷ�
	BOOL insert_friendship(DWORD dw_role_id, LPVOID p_friendship_save);

	// ���º��Ѷ�
	BOOL update_friendship(DWORD dw_role_id, LPVOID p_friendship_save);

	// ɾ�����Ѷ�
	BOOL delete_friendship(DWORD dw_role_id, DWORD dw_friend_id);

	// ɾ����ɫʱ��ɾ�����к��Ѷ����
	BOOL delete_all_friendship_relation(DWORD dw_role_id);
private:
	VOID makesql_insert_friendship(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friendship_save);
	VOID makesql_update_friendship(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friendship_save);
	VOID makesql_delete_friendship(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_friend_id);
	VOID makesql_delete_all_friendship_relation(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);

public:// ������
	
	// �¼Ӻ�����
	BOOL insert_black(DWORD dw_role_id, DWORD dw_black_id);

	// ɾ��������
	BOOL delete_black(DWORD dw_role_id, DWORD dw_black_id);

	// ���к�ĳ���йصĺ�������Ϣ��ɾ��ʱ�����
	BOOL delete_black_all(DWORD dw_role_id);
private:
	VOID makesql_insert_black(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_black_id);
	VOID makesql_delete_black(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_black_id);
	VOID makesql_delete_black_all(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_black_id);
public:// ����

	// �¼ӵ���
	BOOL insert_enemy(DWORD dw_role_id, DWORD dw_enemy_id);
	// ɾ������
	BOOL delete_enemy(DWORD dw_role_id, DWORD dw_enemy_id);
	// ���к�ĳ���йصĳ�����Ϣ��ɾ��ʱ�����
	BOOL delete_enemy_all(DWORD dw_role_id);
private:
	VOID makesql_insert_enemy(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_enemy_id);
	VOID makesql_delete_enemy(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_enemy_id);
	VOID makesql_delete_enemy_all(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_enemy_id);
public:// �ƺ�

	// �����ƺ�
	BOOL insert_title(DWORD dw_role_id, LPVOID p_buffer);
	// ɾ���ƺ�
	BOOL delete_title(DWORD dw_role_id, DWORD dw_title_id);
private:
	VOID makesql_insert_title(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_title(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_title_id);
public:	// ����
	
	// ��������
	BOOL insert_quest(DWORD dw_role_id, LPVOID p_accept_quest);

	// ��������
	BOOL save_quest(DWORD dw_role_id, LPVOID p_task, int n_count, OUT LPVOID *pp_buffer);

	// �����������
	BOOL save_quest_done(DWORD dw_role_id, LPVOID p_task, int n_count);

	// ɾ������
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
	// ��ȡ�ٱ���Ʒ
	BOOL load_item_baibao(DWORD dw_account_id, int n_load, int64 n64_serial, int &n_loaded, int &n_data_size, LPVOID p_buffer);
	// ��ȡ�ٱ�Ԫ��
	BOOL load_baibao_yuanbao(DWORD dw_account_id, int& n_yuanbao);
	// ��ȡ�ٱ���־
	BOOL load_baibao_log_info(DWORD dw_account_id, int &n_loaded, int &n_data_size, LPVOID p_buffer);
	// ��ȡ��log��item����
	BOOL load_item_need_log(LPVOID pNeedLogItem, int &n_item_count);
	// �����ٱ���־
	BOOL insert_baibao_log_info(DWORD dw_account_id, LPVOID p_buffer);
	// ����Ԫ����ֵ
	BOOL update_yuanbao_recharge(DWORD dw_account_id, INT32 n32_yuanbao);
	// ��ȡ��ҳ�콱��־
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

public: // �ʼ�ϵͳ

	// ��ȡ�ʼ���Ʒ
	BOOL load_mail_item(LPVOID p_buffer);

	// �����ʼ���Ʒ
	BOOL update_mail_item(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);

	// �����ʼ�
	BOOL update_mail(NET_DB2C_update_mail* p_update_mail);
	
	// ��ȡ�����ʼ�
	BOOL load_all_mail(LPVOID p_buffer);

	// ��ȡ�ʼ�����
	BOOL load_mail_content(LPVOID p_buffer);

	// �����ʼ�
	BOOL save_mail(NET_DB2C_save_mail* p_save_mail);

	//  ɾ���ʼ�
	BOOL delete_mail(NET_DB2C_delete_mail* p_delete_mail);

	// ��ȡ�ʼ������
	BOOL get_mail_max_serial(DWORD& dw_mail_serial);

	// ���������Ǯ
	BOOL change_role_money(NET_DB2C_change_role_money* p_chang_role_money);

	// ���½�ɫ���ʼ�����
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
	//  ��ȡ���������
	BOOL get_paimai_max_id(DWORD& dw_paimai_id);
	// ����������Ʒ
	BOOL update_paimai_item(DWORD dw_role_id, LPVOID p_buffer, int n_count, OUT LPVOID *pp_buffer);
	// ����������Ϣ
	BOOL insert_paimai(NET_S2DB_insert_paimai* p_paimai);
	// ��ȡ����������Ϣ
	BOOL load_all_paimai(LPVOID p_buffer);
	// ��ȡ�����Զ�������Ϣ
	BOOL load_all_auto_paimai(LPVOID p_buffer);
	// ����Ƿ��Ѿ�����
	BOOL check_is_paimai(DWORD dw_auto_paimai_id);
	// ������������
	BOOL update_auto_paimai(NET_S2DB_update_auto_paimai* p_auto_paimai);
	// ��ȡ������Ʒ
	BOOL load_paimai_item(LPVOID p_buffer);
	// ɾ������
	BOOL delete_paimai(NET_S2DB_delete_paimai* p_recv);
	// ��������
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
	// ��ȡǮׯ�����
	BOOL	get_bank_max_id(DWORD&	dw_bank_id);
	// ����Ǯׯ��Ϣ
	BOOL	insert_bank(NET_S2DB_insert_bank* p_bank);
	// ɾ��Ǯׯ��Ϣ
	BOOL	delete_bank(NET_S2DB_delete_bank* p_bank);
	// ����Ǯׯ
	BOOL	update_bank(NET_S2DB_update_bank* p_bank);
	// ��ȡ����Ǯׯ��Ϣ
	BOOL	load_all_bank(LPVOID p_buffer);

private:
	VOID makesql_insert_bank(odbc::sql_language_disposal* p_stream, DWORD dw_bank_id, LPVOID p_buffer);
	VOID makesql_delete_bank(odbc::sql_language_disposal* p_stream, DWORD dw_bank_id, LPVOID p_buffer);
	VOID makesql_update_bank(odbc::sql_language_disposal* p_stream, DWORD dw_bank_id, LPVOID p_buffer);
	VOID makesql_load_all_bank(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id);
private:
	VOID procres_load_all_bank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// ���а�

	// ��ȡ�ȼ����а�
	BOOL load_level_rank();

	// ��ȡ1v1���ְ�
	BOOL load_1v1_score_rank();

	// ��ȡ�������а�
	BOOL load_guild_rank();

	// ��ȡװ�����а�
	BOOL load_equip_rank(OUT LPVOID &p_buffer, int *p_number);

	// ��ȡ�������а�
	BOOL load_kill_rank();
	
	// ��ȡ�����
	BOOL load_justice_rank();
	//��ȡ�ɻ��
	BOOL load_shihun_rank();
	//�ɾ͵�����
	BOOL load_achpoint_rank();
	//�ɾ�������
	BOOL load_achnumber_rank();
	// ʦͽ�� gx add 2013.12.06
	BOOL load_master_rank();
	//�����
	BOOL load_mounts_rank();
	//��ֵ��
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
	//ʦͽ
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
	//ʦͽ
	VOID procres_load_master_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_mounts_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
	VOID procres_load_reach_rank(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);
public://��Ƭ

	// ��ȡ��ɫ��Ƭ
	BOOL load_visiting_card(LPVOID &p_buffer, DWORD dw_role_id);

	// ������ɫ��Ƭ
	BOOL insert_visiting_card(DWORD dw_role_id);

	// ���½�ɫ��Ƭ
	BOOL update_visiting_card(DWORD dw_role_id, const PVOID p_buffer);

	// ɾ����ɫ��Ƭ
	BOOL delete_visiting_card(DWORD dw_role_id);
private:
	VOID makesql_insert_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, PVOID pDum);
	VOID makesql_update_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_reserved);
	VOID makesql_load_visiting_card(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
private:
	VOID procres_load_visiting_card(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public://����
	
	// ��ȡ��ɫ��������
	BOOL load_clan_data(LPVOID &p_buffer, DWORD dw_role_id);

	// ������������
	BOOL insert_clan_data(DWORD dw_role_id);

	// ɾ����������
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
public:	// ������

	// �����䱦
	BOOL load_act_treasures(LPVOID& p_buffer, int& n_total_num);

	// ��������ü�¼
	BOOL load_fame_hall_snaps(LPVOID& p_buffer, int& n_total_num);

	// ��������ʱ��
	BOOL load_rep_reset_timestamp(LPVOID& p_buffer);

	// ����������
	BOOL load_rep_rank(BYTE b_clan_type, PVOID p_buffer, int &n_count);

	// �������������ü�¼
	BOOL insert_fame_hall_enter_snap(PVOID p_buffer);

	// ���������䱦
	BOOL insert_act_treasure(PVOID p_buffer);

	// ������������ʱ��
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

public://��������

	// ��ȡ���а�������
	BOOL load_all_guild(LPVOID p_buffer, int& n_guild_count);
	
	// ��ȡ��ֲ��Ϣ
	BOOL load_all_guild_plant_data(LPVOID p_buffer, int& n_count);

	// �½�����
	BOOL insert_guild(s_create_guild_info* p_create_info);

	// ɾ������
	BOOL delete_guild(DWORD dw_guild_id);

	// ���ð�ᱨ������
	BOOL resetSignUpAttack();

	// �޸İ�������
	BOOL update_guild_tenet(DWORD dw_guild_id, LPTSTR sz_tenet);

	// �޸İ���ͼ��
	BOOL update_guild_symbol(DWORD dw_guild_id, LPTSTR sz_tenet);

	// ���������������
	BOOL update_guild_all(tagGuildBase *p_guild_base, int n_guild_num);
	BOOL update_guild_all_plant(s_guild_plant *p_guild_plant, int n_guild_num);

	// �����������
	BOOL update_guild_attribute(tagGuildBase *p_guild_base);
	BOOL update_guild_plant(s_guild_plant* p_guild_plant);
	BOOL load_sbk_guild(LPVOID p_buffer, int& n_count);
	BOOL save_guild_sbk(DWORD dw_guild_id);
	// �޸İ���ְλ����
	BOOL update_guild_pos_name(DWORD dw_guild_id, NET_DB2C_change_guild_pos_name *p_guild_pos_name);
	
	// �޸İ���ְλȨ��
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

public://���ɳ�Ա����

	// ��ȡ���а��ɳ�Ա����
	BOOL load_all_guild_member(LPVOID p_buffer, int& n_guil_member);

	// �¼Ӱ��ɳ�Ա
	BOOL insert_guild_member(const s_create_guild_member_info& create_member_info);

	// ���ɳ�Ա���߻����Լ����
	BOOL delete_guild_member(DWORD dw_role_id);

	// ���°���ְλ
	BOOL update_guild_member_pos(DWORD dw_role_id, INT8 n8_pos);

	// ���ð��ɳ�Ա���ʲֿ�Ȩ��
	BOOL guild_ware_privilege(DWORD dw_role_id, BOOL b_enable);

	// ���İ��ɳ�Ա���׶�
	BOOL change_member_contribution(LPVOID p_buffer);

	// ���ð��ɳ�ԱͶƱȨ��
	BOOL change_ballot(LPVOID p_buffer);
	
	// �����Ƿ��ս
	//BOOL change_war(LPVOID p_buffer);

	// ���°��ɳ�ԱDKP
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

public:// ���ɲֿ�

	// ��ȡ���ɲֿ����е���
	BOOL load_guild_ware_all_items(LPVOID &p_buffer, DWORD dw_guild_id, int& n_item);
private:
	VOID makesql_load_guild_ware_items(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id);

public:// ������ʩ

	// ��ȡ����������ʩ
	BOOL load_guild_facilities(LPVOID p_buffer, int& n_info, DWORD dw_guild_id);

	// ����������ʩ
	BOOL insert_guild_facilities(s_facilities_load* p_info);

	// ���°�����ʩ
	BOOL update_guild_facilities(s_facilities_load* p_info);

	// ɾ��������ʩ
	BOOL delete_guild_facilities(DWORD dw_guild_id);
private:
	VOID makesql_load_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, DWORD dw_role_id);
	VOID makesql_insert_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_guild_facilities(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
private:
	VOID procres_load_guild_facilities(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public://����pvp

	// �½�����PVP����
	BOOL insert_guild_pvp_data(int n_act_id, NET_DB2C_insert_pvp_data* p_pvp);

	// ȡ�ð���PVP����
	BOOL load_guild_pvp_data(LPVOID p_buffer, int& nDataNum);

	// ���°���PVP����
	BOOL update_pvp_data(int n_act_id, NET_DB2C_up_pvp_data* p_pvp);
private:
	VOID makesql_insert_guild_pvp_data(odbc::sql_language_disposal* p_stream, DWORD dwActID, LPVOID p_buffer);
	VOID makesql_load_guild_pvp_data(odbc::sql_language_disposal* p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_update_pvp_data(odbc::sql_language_disposal* p_stream, DWORD dwActID, LPVOID p_buffer);
private:
	VOID procres_load_guild_pvp_data(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:
	// ��ȡ�����ļ����Ϣ
	BOOL load_guild_recruit(LPVOID p_buffer);
	// ��Ӱ����ļ��Ϣ
	BOOL insert_guild_recruit(DWORD dw_role_id);
	// ɾ�������ļ��Ϣ
	BOOL delete_guild_recruit(DWORD dw_role_id);
	// ��������ļ��
	BOOL clean_guild_recruit();
private:
	VOID makesql_load_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, DWORD dw_account_id);
	VOID makesql_insert_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_delete_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_clean_guild_recruit(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);

private:
	VOID procres_load_guild_recruit(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:
	// ��ȡ���boss��ɱ
	BOOL load_guild_boss_kill(LPVOID p_buffer);
	BOOL insert_guild_kill_boss(s_guild_kill_boss* p_data);
	// ���ս��
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

public:// ���ɼ���

	// ��ȡ�������м���
	BOOL load_guild_skill_info(LPVOID p_buffer, DWORD dw_guild_id, int& n_count);

	// ������Ἴ��
	BOOL insert_guild_skill_info(DWORD dw_guild_id, s_guild_skill_info* pSkillInfo);

	// ���°��ɼ���
	BOOL update_guild_skill_info(DWORD dw_guild_id, s_guild_skill_info* pSkillInfo);

	// ���İ����о�����
	BOOL change_research_guild_skill(DWORD dw_guild_id, DWORD dw_skill_id);

	// ɾ����Ἴ��
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

public:// ���ɵ���

	// ��ȡ���ɵ���
	BOOL load_guild_delate(LPVOID p_buffer, DWORD dw_guild_id);

	// �������ɵ���
	BOOL insert_guild_delate(DWORD dw_guild_id);

	// ɾ�����ɵ���
	BOOL delete_guild_delate(DWORD dw_guild_id);

	// ���°��ɵ���
	BOOL update_guild_delate(LPVOID p_buffer, DWORD dw_guild_id);

	// ���°��ɵ�������
	BOOL update_guild_delate_content(DWORD dw_guild_id, LPTSTR szNewTenet);
private:
	VOID makesql_load_guild_delate(odbc::sql_language_disposal * p_stream, DWORD dw_guild_id, DWORD dw_role_id);
	VOID makesql_insert_guild_delate(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_delete_guild_delate(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_delate(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
	VOID makesql_update_guild_delate_content(odbc::sql_language_disposal *p_stream, DWORD dw_guild_id, LPVOID p_buffer);
private:
	VOID procres_load_guild_delate(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// ��������

	// ��ȡ�����̻�����
	BOOL load_guild_commerce_info(LPVOID p_buffer, DWORD dw_guild_id, int& n_count);

	// ��ȡ�����̻����а�
	BOOL load_commerce_rank_info(LPVOID p_buffer, DWORD dw_guild_id, int& n_count);

	// ������Ʒ��Ϣ
	BOOL insert_commodity_info(LPVOID p_buffer);

	// ������Ʒ��Ǯ
	BOOL update_commodity_money(DWORD dw_role_id, int n_money);

	// ������Ʒ��Ϣ
	BOOL save_commodity_info(LPVOID p_buffer, DWORD dw_role_id);

	// ɾ����Ʒ��Ϣ
	BOOL delete_commodity_info(DWORD dw_role_id);

	// ���ð��ɱ���
	BOOL set_guild_citation(DWORD dw_guild_id, BOOL b_citation);

	// �����̻�����
	BOOL update_commerce_rank_info(DWORD dw_guild_id, LPVOID p_buffer);

	// ɾ���̻�����
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

public:// �������
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

public:// �̳����
	
	// ��ȡ�����Ź���Ϣ
	BOOL load_all_group_purchase_info(OUT PVOID p_buffer, int &n_info_number);

	// �����Ź���Ϣ
	BOOL add_group_purchase_info(s_gp_info* p_gp_info);

	// ɾ���Ź���Ϣ
	BOOL delete_group_purchase_info(s_gp_info_key* p_gp_info_key);

	// ɾ�������Ź���Ϣ
	BOOL delete_guild_group_purchase_info(DWORD dw_guild_id);

	// �����Ź���Ϣ
	BOOL update_group_purchase_info(s_gp_info_key* p_gp_info_key, DWORD dwNewParticipator);

	// �����Ź�ʱ��
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

public:// VIP̯λ
	// ��ȡVIP̯λ
	BOOL get_all_vip_stall_info(OUT PVOID p_buffer);

	// VIP̯λ����
	BOOL apply_vip_stall(s_vip_stall_to_db* pInfo);

	// VIP̯λʣ��ʱ��
	BOOL update_vip_stall_time();
private:
	VOID makesql_get_all_vip_stall_info(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
	VOID makesql_apply_vip_stall(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_vip_stall_time(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_insert_vip_stall(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID procres_get_all_vip_stall_info(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// ��������
	BOOL update_vip_net_bar_player(DWORD dw_account_id, DWORD dw_login_time);
	BOOL load_vip_net_bar(OUT PVOID p_buffer, DWORD dw_date);
private:
	VOID makesql_load_history_vip_net_bar_players(odbc::sql_language_disposal *p_stream, DWORD dw_date, DWORD dw_data);
	VOID makesql_load_today_vip_net_bar_players(odbc::sql_language_disposal *p_stream, DWORD dw_date, DWORD dw_data);
	VOID makesql_update_vip_net_bar_player(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, PVOID p_buffer);
private:
	VOID procres_load_vip_net_bar_players(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// ����

	// ��ȡ����
	BOOL load_leave_msg(DWORD dw_role_id, PVOID p_leave_msg, DWORD &dw_size);

	// ��ȡ�����������
	BOOL load_leave_msg_indexes(DWORD* p_indexes, DWORD dw_role_id, int &n_count);

	// ɾ������
	BOOL delete_leave_msg(DWORD dw_role_id, DWORD dwDateTime);

	// ��������
	BOOL insert_leave_msg(DWORD dw_role_id, DWORD dwDateTime, PVOID pLeftMsg);

	// �������
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

public://��������ͳ��

	// ���±�����Ŀ
	BOOL update_treasure_count(DWORD dw_role_id, int& nSum);
private:
	VOID makesql_update_treasure_count(odbc::sql_language_disposal* p_stream, DWORD dw_role_id, LPVOID p_buffer);

public: //ʦͽϵͳ

	// ��ȡʦͽ����
	BOOL load_all_master_prentices( OUT LPVOID& pBuffer, OUT int& n32Size );

	// ��ȡʦͽ��
	BOOL load_all_master_recruit(LPVOID pBuffer, OUT INT& n32Size);

	// ����ʦͽ����
	BOOL save_master_prentices(LPVOID p_buffer, DWORD dw_role_id, UINT32 u32Number);

	// ����ɫ����ʦͽ��
	BOOL add_new_master_prentices( DWORD dw_role_id, LPVOID p_buffer = NULL );

	// ����ʦ��ID�ͽ�ֹ����ʱ��
	BOOL update_master_id_and_forbid_time( DWORD dw_role_id, LPVOID p_buffer );

	// ʦ��ɾ����ɫ
	BOOL master_delete_role( DWORD dw_role_id, LPVOID p_buffer = NULL );

	// �����ʦ��
	BOOL join_master_recruit(DWORD dw_role_id, LPVOID p_buffer = NULL);

	// �˳���ʦ��
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

public:// �ϵͳ

	// ��ȡ���л����
	BOOL load_activities(LPVOID p_buffer, int &n_count);

	// �洢ĳ�������
	BOOL save_activity(DWORD dw_id, LPVOID pActivityData);
private:
	VOID makesql_save_activity(odbc::sql_language_disposal *p_stream, DWORD dw_id, LPVOID p_activity_data);
	VOID makesql_load_activities(odbc::sql_language_disposal *p_stream, DWORD dw_account_id, DWORD dw_role_id);
private:
	VOID procres_load_activities(OUT LPVOID &p_buffer, OUT int *p_number, odbc::execute_result *p_result);

public:// ��־��¼

	// ��Ǯ��־
	BOOL create_log_money_table(LPCSTR sz_table_postfix);

	// Ԫ����־
	BOOL create_log_yuanbao_table(LPCSTR sz_table_postfix);

	// ������־
	BOOL create_log_exvolume_table(LPCSTR sz_table_postfix);

	// ս����־
	BOOL create_log_exploits_table(LPCSTR sz_table_postfix);

	// ��ʱ��־
	BOOL create_log_time_stat_table(LPCSTR sz_table_postfix);

	// ��Ʒ��־
	BOOL create_log_item_table(LPCSTR sz_table_postfix);

	// ��Ʒ������־
	BOOL create_log_item_times_table(LPCSTR sz_table_postfix);

	// �̳ǳ�����־
	BOOL create_log_mall_sell_table(LPCSTR sz_table_postfix);

	// �̳ǳ��۵������־
	BOOL create_log_mall_sell_pack_table(LPCSTR sz_table_postfix);

	// ���ɻ�����־
	BOOL create_log_fund_table(LPCSTR sz_table_postfix);

	// ������־
	BOOL create_log_material_table(LPCSTR sz_table_postfix);

	//������־
	BOOL create_log_reputation_table(LPCSTR sz_table_postfix);

	// GM ָ����־
	BOOL create_log_gm_cmd_table(LPCSTR sz_table_postfix);
	
	// Ǯׯ��־
	BOOL create_log_bank_table(LPCSTR sz_table_postfix);

	// ������־
	BOOL create_log_paimai_table(LPCSTR sz_table_postfix);

	// ����������־
	BOOL create_log_leave_practice_table(LPCSTR sz_table_postfix);
	// ���к����
	BOOL create_log_serial_reward(LPCSTR sz_table_postfix);

	// ����������־
	BOOL create_log_kick(LPCSTR sz_table_postfix);
public:
	// ������־��¼
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
	// ������־����
	BOOL common_create_log_table(LPCSTR str_table, LPCSTR sz_table_postfix);
public:
	// �������Ԫ��ֵ
	BOOL reset_all_role_vigour( DWORD dw_role_id, LPVOID p_buffer );
	// ��������
	BOOL clean_all_role_luck( DWORD dw_role_id, LPVOID p_buffer );

	BOOL clear_destory_equip_count( DWORD dw_role_id, LPVOID p_buffer );

	BOOL clean_shihun(DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID makesql_reset_all_role_vigour(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_clean_all_role_luck(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_clear_destory_equip_count(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_clean_shihun(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
public:
	// 1v1���־�����
	BOOL update_role_1v1_score(DWORD dw_role_id, LPVOID p_buffer);
	BOOL update_noline_1v1_score(DWORD dw_role_id, LPVOID p_buffer);

	BOOL update_mounts(DWORD dw_role_id, LPVOID p_buffer);
private:
	VOID makesql_update_role_1v1_score(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
	VOID makesql_update_noline_1v1_score(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);

	VOID makesql_update_mounts(odbc::sql_language_disposal *p_stream, DWORD dw_role_id, LPVOID p_buffer);
public:
	// ��������
	BOOL load_all_guerdonquest(OUT LPVOID p_buffer, OUT INT& nNumber);
	BOOL load_all_gdreward(OUT LPVOID& p_buffer, OUT INT& nNumber);
	VOID insert_new_guerdonquest(PVOID p_buffer);
	VOID update_guerdonquest(PVOID p_buffer);
	VOID delete_guerdonquest(PVOID p_buffer);
	VOID update_questcompletedata(PVOID p_buffer);
	VOID update_quest_flag(PVOID p_buffer);
	VOID insert_quest_rewarditem(PVOID p_buffer);
	VOID delete_quest_rewarditem(PVOID p_buffer);

	// ����sns
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
	// ��ȡroledata��¼���������RoleID
	VOID get_role_count_and_max_role_id(int &n_count, DWORD &dw_max_role_id);

	// ��ȡ�������������ID
	VOID get_pet_count_and_max_pet_id(int &n_count, DWORD &dw_max_pet_id);

	// ��ȡԪ���˻�����
	VOID get_yuanbao_account_count(int &n_account_count);

	// ��ȡԪ����������
	VOID get_yuanbao_order_count(int &n_order_count);

	// ��û����
	VOID get_activity_count(int &n_activity_count);

	// ��ȡ������С���к�
	VOID get_world_max_min_serial(const char* sz_table, int64 &n64Max, int64 &n64Min);

	// ��ȡ�ʼ����ID
	VOID get_max_mail_serial(const char* sz_table, DWORD& dwMax);

	// ��ȡ�Ź����������ݴ�С
	VOID get_group_purchase_info_count_size(int &n_gp_info_number, int &n_data_size);

public:
	// �����ݿ����
	DWORD get_total_read_count() const { return dw_read_count_; }

	// д���ݿ����
	DWORD get_total_write_count() const { return dw_write_count_; }

	// ��ý�ɫ����
	int get_role_count() const { return n_role_count_; }

	// ���Ԫ���˻�����
	int get_account_count() const { return n_account_count_; }

	// ���Ԫ����������
	int get_yuanbao_order_count() const { return n_order_count_; }

	// ���Ԫ���������ID
	DWORD get_yuanbao_max_order_id() const	{ return dw_max_order_index_; }

	// ���Ԫ�������������
	VOID get_yuanbao_max_order_index(DWORD& dw_max_index);

	// ��ý�ɫԪ��������
	VOID get_one_role_yuanbao_order_count(DWORD dw_role_id, int &n_order_number);

	// ��û����
	int get_activity_count() const { return n_activity_count_; }

	// ������Ʒ����Ψһ���/��С����
	VOID get_world_max_min_serial(int64 &n64_max, int64 &n64_min);

	int get_group_purchase_info_count() const { return n_group_purchase_info_count_; }
	int get_group_purchate_info_size() const { return n_group_purchase_info_size_; }

	VOID reset_load_count_limit(int n_load_limit)	{ n_load_count_limit_ = n_load_limit; }
	VOID reset_load_serial(int64 n64_serial)	{ n64_load_baibao_item_serial = n64_serial; }

public:// ���ݿ�ѹ������
	BOOL force_test_load_role(LPVOID &p_role_data, DWORD dw_account_id, DWORD dw_role_id)
	{
		return load_one_role_data(p_role_data, dw_account_id, dw_role_id);
	}
	BOOL force_test_load_item(LPVOID &p_buffer, int &n_count, DWORD dw_account_id, DWORD dw_role_id)
	{
		return load_one_role_items(p_buffer, n_count, dw_account_id, dw_role_id);
	}


public: // ��(���)����
	BOOL lock_tables(LPCSTR szTableNames);
	BOOL unlock_tables();
private:
	VOID makesql_lock_tables(odbc::sql_language_disposal *p_stream, DWORD dw_reserve, LPVOID p_buffer);
	VOID makesql_unlock_tables(odbc::sql_language_disposal *p_stream, DWORD dw_reserve, LPVOID p_buffer);

private: 
	// ��������,ͬ������
	BOOL common_select(
						OUT LPVOID &p_buffer, 
						OUT int *p_number, 
						DWORD dw_account_id, 
						DWORD dw_role_id, 
						func_param3 p_func_format, 
						func_result_proc p_func_procres
	);

	// ��������(ɾ��/����/����),������,ͬ������
	BOOL common_update(
						DWORD dw_role_id, 
						LPVOID p_buffer, 
						int n_count, 
						int nSzOne, 
						func_param3_1 p_func_format, 
						OUT LPVOID *pp_buffer = NULL
	);

	// ��־����,��������,�첽����(ɾ��/����/����)
	VOID common_update_log(
							LPCSTR sz_table_postfix, 
							LPVOID p_buffer, 
							func_param3_2 p_func_format
	);
private:
	// ��ʼ����Ʒ�����ֶ�
	VOID init_item_load_field(odbc::sql_language_disposal *p_stream);

private:
	
	few_connect_server* server_;
	db_server*			p_db_server_;

	// ��ɫ����
	int	n_role_count_;
	// ����ɫID
	DWORD dw_max_role_id_;
	// �˺�����
	int	n_account_count_;

	// ��������
	int	n_order_count_;
	// ��󶩵�����
	DWORD dw_max_order_index_;
	// һ�ζ�ȡ�����
	int   n_load_count_limit_;
	// �ٱ�������Ʒ���ID
	int64 n64_load_baibao_item_serial;

	// �Ź������
	int	n_group_purchase_info_count_;
	// �Ź���Ϣ�ڴ��С
	int	n_group_purchase_info_size_;

	// ������ID
	DWORD dw_max_pet_id_;
	// �������
	int n_pet_count_;

	// �����
	int n_activity_count_;

	// ƴ��SQL���ʱ��ʱʹ�õ�buffer
	char sz_time_buffer_[X_DATATIME_LEN + 1];
	// ��ɫ��CRC32ʱʹ��
	TCHAR sz_role_name_buffer_[64];

	// �Ƿ��Ѿ��������ݿ�
	volatile BOOL b_is_init_ok_;
	// �����ݿ����
	volatile DWORD dw_read_count_;
	// д���ݿ����
	volatile DWORD dw_write_count_;

	// ��Ϸ�����������ݿ�ӿ�
	odbc::db_interface*	game_db_interface_;
	// ��־���ݿ�ӿ�
	odbc::db_interface*	log_db_interface_;
	// �������ݿ�ӿ�
	odbc::db_interface* user_number_db_interface;
};
 
#endif //__DATABASE_HANDLRR_H__
