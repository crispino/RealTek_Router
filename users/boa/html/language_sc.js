﻿
var language_sel='语言选择:';
var language_sc='简体中文';
var language_en='English';
/***********	menu.htm	************/
var menu_site ='网站内容';
var menu_setup ='网络模式';
var menu_wizard ='设置向导';
var menu_opmode ='模式设置';
var menu_wireless='无线';
var menu_wlan1='无线5G 设置';
var menu_wlan2='无线2.4G 设置';
var menu_basic='基本设置';
var menu_advance='高级设置';
var menu_security='安全';
var menu_8021xCert='802.1x 证书安装';
var menu_accessControl='访问控制';
var menu_wds='WDS设置';
var menu_mesh='网格设置';
var menu_siteSurvey='站点扫描';
var menu_wps='WPS';
var menu_schedule='时间表';
var menu_tcpip='TCP/IP 设置';
var menu_lan='局域网设置';
var menu_wan='广域网设置';
var menu_fireWall='防火墙';
var menu_portFilter='端口过滤';
var menu_ipFilter='IP地址过滤';
var menu_macFilter='MAC地址过滤';
var menu_portFw='端口转发';
var menu_urlFilter='URL过滤';
var menu_dmz='隔离区';
var menu_vlan='虚拟局域网';
var menu_qos='服务质量控制';
var menu_routeSetup='路由设置';
var menu_management='管理';
var menu_status='状态';
var menu_statistics='统计信息';
var menu_ddns='动态域名服务';
var menu_andlink='andlink配置'
var menu_timeZone='时区设置';
var menu_dos='拒绝服务攻击';
var menu_log='日志记录';
var menu_updateFm='升级固件';
var menu_setting='保存/加载设置';
var menu_psw='设置密码';
var menu_logout='注销';
var menu_cpu = 'CPU 应用';
var menu_tr69_config = 'TR-069 配置';
var menu_cpu = 'BT 传输';
var menu_weave_set = 'Weave 设置';
var menu_zigbee = 'ZigBee 管理';
var menu_openvpn = 'Openvpn 设置';
var menu_diagnostic = '系统诊断';

/***********	opmode.htm	************/
var opmode_header='模式设置';
var opmode_explain='您可以对LAN和WAN设置NAT/bridge功能';
var opmode_radio_gw='网关:';
var opmode_radio_gw_explain= '在该模式下，设备通过ADSL或者调制解调器连接到因特网。NAT是使能的，并且局域网端口的主机通过广域网端口共享同一个IP连接到因特网的服务提供商。在广域网页面下可以设置不同的连接类型，类型包括了PPPOE，DHCP客户端，PPTP客户端，L2TP客户端，静态IP。';
var opmode_radio_br='桥接:';
var opmode_radio_br_explain= '在该模式下，所有的以太网端口和无线接口桥接在一起，且NAT功能是不使能的。该模式同样不支持所有与广域网端口有关的功能及防火墙';
var opmode_radio_rp='中继:';
var opmode_radio_rp_explain='在该模式下，所有的以太网端口和无线接口桥接在一起，并且无线客户端将连接到远程接入点。您可以在网络扫描页面连接到远程接入点.';

var opmode_radio_wisp='无线网络服务提供商:';
var opmode_radio_wisp_explain= '在该模式下，所有的以太网端口桥接在一起，无线客户端连接到网络服务提供商的接入点。NAT是使能的，且以太网端口的主机通过无线局域网共享同一个IP连接到网络服务提供商。在网络扫描页面下可以连接到网络服务提供商的接入点。在广域网页面下可以设置不同的连接类型，类型包括了PPPOE，DHCP客户端，PPTP客户端，L2TP客户端，静态IP。';
var opmode_radio_wisp_wanif = 'WAN接口: ';

var opmode_apply='设置';
var opmode_reset='重置';
var opmode_save='保存';
var opmode_save_apply='保存&生效';


/***********	wlan_schedule.htm	************/
var wlan_schedule_header = '无线开关定时';
var wlan_schedule_explain = '您可以设置无线定时，配置无线定时需在管理->时区设置页面中启动NTP客户端更新。';
var wlan_schedule_enable = '启用无线开关定时';
var wlan_schedule_everyday = '每天';
var wlan_schedule_sun = '星期天';
var wlan_schedule_mon = '星期一';
var wlan_schedule_tue= '星期二';
var wlan_schedule_wed = '星期三';
var wlan_schedule_thu = '星期四';
var wlan_schedule_fri = '星期五';
var wlan_schedule_sat = '星期六';
var wlan_schedule_24Hours = '24小时';
var wlan_schedule_from = '从';
var wlan_schedule_to = '到';
var wlan_schedule_save = '设置';
var wlan_schedule_reset = '重置';
var wlan_schedule_days = '天';
var wlan_schedule_time = '时间';
var wlan_schedule_time_check = '请设置天!';
var wlan_schedule_ena='开启';
var wlan_schedule_day='天';
var wlan_schedule_for_save='保存';
var wlan_schedule_save_apply = '保存&生效';
var wlan_schedule_for_hour='(小时)';
var wlan_schedule_for_min='(分钟)';
var wlan_schedule_rule='时间表规则';
var wlan_schedule_sbte='错误，开始时间大于结束时间';
var wlan_schedult_sequale="错误. 开始时间等于结束时间"
/***********	wlwps.htm	************/
var wlwps_header = 'Wi-Fi保护设置';
var wlwps_header_explain = '您可以改变WPS(Wi-Fi保护)的设置，该功能能够使您的无线设备快速连接到AP并同步AP的设置。';
var wlwps_wps_disable = '禁用WPS';
var wlwps_wps_save = '设置';
var wlwps_wps_reset = '重置';

var wlwps_save = '保存';
var wlwps_save_apply = '保存&生效';

var wlwps_status = 'WPS状态:';
var wlwps_status_conn = '已配置';
var wlwps_status_unconn = '未配置';
var wlwps_runon = 'WPS接口:';
var wlwps_runon_root = '主接口';
var wlwps_runon_rpt = '虚接口';
var wlwps_status_reset = '重置到未配置';
var wlwps_lockdown_state = '自动锁定状态';
var wlwps_self_pinnum = '自己的PIN :';
var wlwps_unlockautolockdown = '解锁';
var wlwps_lockdown_state_locked = '锁定';
var wlwps_lockdown_state_unlocked = '未锁定';
var wlwps_pbc_title = '按键配置:';
var wlwps_pbc_start_button = '开始配置';
var wlwps_stopwsc_title = '停止WSC';
var wlwps_stopwsc_button = '停止WSC';
var wlwps_pin_number_title = '客户PIN码:';
var wlwps_pin_start_button = '开始PIN';
var wlwps_keyinfo = '当前密匙信息:';
var wlwps_authentication = '认证';
var wlwps_authentication_open = '打开';
var wlwps_authentication_wpa_psk = 'WPA PSK';
var wlwps_authentication_wep_share = 'WEP共享';
var wlwps_authentication_wpa_enterprise = '企业级WPA模式';
var wlwps_authentication_wpa2_enterprise = '企业级WPA2模式';
var wlwps_authentication_wpa2_psk = 'WPA2 PSK';
var wlwps_authentication_wpa2mixed_psk = 'WPA2混杂模式 PSK';
var wlwps_encryption = '加密';
var wlwps_encryption_none= '无';
var wlwps_encryption_wep = 'WEP';
var wlwps_encryption_tkip = 'TKIP';
var wlwps_encryption_aes = 'AES';
var wlwps_encryption_tkip_aes = 'TKIP+AES';
var wlwps_key = '密匙';
var wlwps_pin_conn = 'PIN配置:';
var wlwps_assign_ssid = '分配的SSID:';
var wlsps_assign_mac = '分配的MAC:';
var wlwps_wpa_save_pininvalid = '无效的PIN长度! 设备PIN通常是4或8位长.';
var wlwps_wpa_save_pinnum = '无效的PIN! 设备PIN必须为数字.';
var wlwps_wpa_save_pinchecksum = '无效的PIN，校验和错误.';
var wlwps_pinstart_pinlen = '无效的初始PIN长度! 设备PIN通常是4或8位长.';
var wlwps_pinstart_pinnum = '无效的初始PIN! 初始PIN必须是数字.';
var wlwps_pinstart_pinchecksum = '校验和错误! 必须使用PIN? ';
var wlwps_macaddr_nocomplete = '输入的MAC地址不完整，MAC地址必须是12个16进制的长度.';

var warn_msg1='因为无线模式设置不被支持,因此WPS被禁用. ' + '必须去无线网络/基本配置界面去改变配置从而启用WPS.';
var warn_msg2='因为Radius认证不被支持,因此WPS被禁用.' +'必须去无线网络/安全性界面去改变设置从而启用WPS.';
var warn_msg3 = 'PIN值被创建.必须单击设置按钮使其生效';
var wlwps_start_pbc_successfully='成功开启PBC<br><br>';
var wlwps_in='在';
var wlwps_client="客户端"
var wlwps_run_wps=',您必须在两分钟之内开启Wi-Fi保护设置';
var wlwps_start_pin='成功开启PIN<br><br>';
var wlwps_apply_wps_pin_successfully='成功应用Wi-Fi保护设置PIN码<br><br>';
var wlwps_runwps_within_2minutes='您必须在两分钟之内开启Wi-Fi保护设置';
var wlwps_apply_wps_stop='成功停止Wi-Fi保护设置!<br>';
var wlwps_apply_wps_unlock='成功将Wi-Fi保护设置解锁!<br>';

/***********	wlsurvey.htm	************/
var wlsurvey_onewlan_header = '无线网络扫描</p>';
var wlsurvey_morewlan_header = '无线网络扫描 -wlan';
var wlsurvey_header_explain = '该页面用于信道扫描并显示扫描到的AP信息';
var wlsurvey_site_survey = '扫描';
var wlsurvey_chan_next = '<input type="button" value="  前进>>" id="next" onClick="saveClickSSID()">';
var wlsurvey_encryption = '加密:';
var wlsurvey_encryption_no = 'Open';
var wlsurvey_encryption_wep = 'WEP';
var wlsurvey_encryption_wpa = 'WPA';
var wlsurvey_encryption_wpa2= 'WPA2';
var wlsurvey_encryption_wpa_mixed = 'WPA-MIXED';
var wlsurvey_keytype = '密匙类型:';
var wlsurvey_keytype_open = '开放';
var wlsurvey_keytype_shared = '共享';
var wlsurvey_keytype_both = '自动';
var wlsurvey_keylen = '密匙长度:';
var wlsurvey_keylen_64 = '64比特';
var wlsurvey_keylen_128 = '128比特';
var wlsurvey_keyfmt = '密匙格式:';
var wlsurvey_keyfmt_ascii = 'ASCII';
var wlsurvey_keyfmt_hex = 'Hex';
var wlsurvey_keyset = '密匙设置';
var wlsurvey_authmode = '验证模式:';
var wlsurvey_authmode_enter_radius = '企业级 (RADIUS)';
var wlsurvey_authmode_enter_server = '企业级(作为服务器)';
var wlsurvey_authmode_personal = '个人 (共享密匙)';
var wlsurvey_ieee_80211w='IEEE 802.11w:';
var wlsurvey_80211w_none='无';
var wlsurvey_80211w_capable='可选';
var wlsurvey_80211w_required='必须';
var wlsurvey_sha256='SHA256';
var wlsurvey_sha256_disable='关闭';
var wlsurvey_sha256_enable='开启';

var wlsurvey_wpacip = 'WPA密钥套件:';
var wlsurvey_wpacip_tkip = 'TKIP';
var wlsurvey_wpacip_aes = 'AES';
var wlsurvey_wp2chip = 'WPA2密钥套件:';
var wlsurvey_wp2chip_tkip = 'TKIP';
var wlsurvey_wp2chip_aes = 'AES';
var wlsurvey_preshared_keyfmt = '预共享密匙格式:';
var wlsurvey_preshared_keyfmt_passphrase = '口令';
var wlsurvey_preshared_keyfmt_hex = 'HEX (64字符)';
var wlsurvey_preshared_key = '预共享密匙:';
var wlsurvey_eaptype = 'EAP类型:';
var wlsurvey_eaptype_md5 = 'MD5';
var wlsurvey_eaptype_tls = 'TLS';
var wlsurvey_eaptype_peap = 'PEAP';
var wlsurvey_intunn_type = '内部隧道类型:';
var wlsurvey_intunn_type_MSCHAPV2 = 'MSCHAPV2';
var wlsurvey_eap_userid = 'EAP用户ID:';
var wlsurvey_radius_passwd = 'RADIUS用户密码';
var wlsurvey_radius_name = 'RADIUS用户名:';
var wlsurvey_user_keypasswd = '用户密钥密码(如果存在):';


var wlsurvey_use_as_server = '用户本地AS服务器:';
var wlsurvey_as_ser_ipaddr = 'AS 服务器;IP 地址:';
var wlsurvey_back_button = '<<后退  ';
var wlsurvey_conn_button = '连接';
var wlsurvey_wait_explain = '请等待...';
var wlsurvey_inside_nosupport = '内部类型不支持.';
var wlsurvey_eap_nosupport = 'EAP类型不支持.';
var wlsurvey_wrong_method = '错误的方法!';
var wlsurvey_nosupport_method = '错误: 不支持方法ID';
var wlsurvey_nosupport_wpa2 = '错误: 不支持wpa2密码套件 ';
var wlsurvey_nosupport_wpasuit = '错误: 不支持wpa密码套件 ';
var wlsurvey_nosupport_encry = '错误: 不支持加密 ';
var wlsurvey_tbl_ssid = 'SSID';
var wlsurvey_tbl_bssid = 'BSSID';
var wlsurvey_tbl_chan = '信道';
var wlsurvey_tbl_type = '类型';
var wlsurvey_tbl_ency = '加密算法';
var wlsurvey_tbl_signal = '信号强度';
var wlsurvey_tbl_select = '选择';
var wlsurvey_tbl_macaddr = 'MAC地址';
var wlsurvey_tbl_meshid = 'Mesh ID';
var wlsurvey_tbl_none = '无';
var wlsurvey_list_mesh='多跳网络节点列表'

var wlsurvey_read_site_error = '扫描状态读取失败!';
var wlsurvey_get_modemib_error = '获取MIB_WLAN_MODE的MIB值失败!';
var wlsurvey_get_bssinfo_error = '获取bssinfo失败!';
var wlsurvey_auto_scan_running='自动扫描正在运行!!请等待...'
var wlsurvey_site_survey_failed='站点扫描请求失败!'
var wlsurvey_scan_request_timout='扫描请求超时!'
var wlsurvey_scan_timeout='扫描超时!'
var wlsurvey_please_refresh_again='请再次刷新!'
var wlsurvey_connect_failed1='连接失败1!'
var wlsurvey_get_meshid_error='获取MIB_WLAN_MESH_ID错误!'
var wlsurvey_connect_failed2='连接失败2!!'
var wlsurvey_set_meshid_error='设置MeshID错误!'
var wlsurvey_set_channel_error='设置信道错误!'
var wlsurvey_set_wpa_error='设置WPA错误!'
var wlsurvey_connect_failed3='连接失败3!!'
var wlsurvey_set_networktype_failed='设置MIB_WLAN_NETWORK_TYPE错误!'
var wlsurvey_set_channelnum_error='设置信道编号错误!'
var wlsurvey_get_join_result_failed='获取加入结果失败!'
var wlsurvey_network_key_mismatch='网络安全密钥不匹配!'
var wlsurvey_connect_failed4='连接失败 4!'
var wlsurvey_connect_timeout='连接超时!'
var wlsurvey_connect_successfully='连接成功!'
var wlsurvey_connect_success_reboot='连接成功! 请等待重新启动.'
var wlsurvey_mic_error='MIC错误, 请检查密码!'
var wlsurvey_connect_addto_profile='添加至无线配置'
var wlsurvey_connect_save='保存'
var wlsurvey_connect_save_apply='保存&生效'
var wlsurvey_connect_ok='确认'
var wlsurvey_reboot_now='立即重启'
var wlsurvey_reboot_later='稍后重启'
var wlsurvey_change_setting_success='成功改变设置!'
var wlsurvey_donot_turn_off='请不要在此期间关闭或重启设备.'
var wlsurvey_please_wait='请等待'
var wlsurvey_seconds='秒'

/***********	wlwds.htm	************/
var wlwds_onelan_header = 'WDS设置</p>';
var wlwds_morelan_header = 'WDS设置 -wlan';
var wlwds_header_explain = '无线分布式系统通过无线介质与其他AP相联.为了实现这个功能,您必须首先将AP设置为WDS或AP+WDS模式下，之后将该AP与其他AP设置在同一信道，最后启用WDS,并设置想进行通信的AP的MAC地址.';
var wlwds_enable = '启用WDS';
var wlwds_mac_addr = 'MAC地址';
var wlwds_data_rate = '数据速率';
var wlwds_rate_auto = '自动';

var wlwds_comment = '说明:';
var wlwds_apply = '设置';
var wlwds_reset = '重置';

var wlwds_save = '保存';
var wlwds_save_apply = '保存&生效';

var wlwds_set_secu = '安全设置';
var wlwd_show_stat = '显示统计值';
var wlwds_wdsap_list = '当前无线分布式系统接入点列表:';
var wlwds_delete_select = '删除所选';
var wlwds_delete_all = '删除全部';
var wlwds_fmwlan_txrate = '数据速率(Mbps)';
var wlwds_fmwlan_select = '选择';
var wlwds_fmwlan_comment = '说明';
var wlwds_macaddr_nocomplete = '输入的MAC地址不完整，MAC地址必须是12个16进制的长度.';
var wlwds_macaddr_invalid = '无效的MAC地址,MAC地址必须是16进制.';
var wlwds_delete_chick = '确定删除所选的入口?';
var wlwds_delete_allchick = '确定删除所有的入口?';
var wlwds_set_enable_flag="设置启用标志错误!"
var wlwds_invalid_mac_address="错误! 无效的MAC地址."
var wlwds_comment_length_too_long='错误! 说明长度太长.'
var wlwds_get_entry_number='获取表格数量错误!'
var wlwds_cannot_add_new_entry="表已满，不能添加新的表项!"
var wlwds_rule_exist="规则已经存在!"
var wlwds_add_table_entry_error="添加表项错误!"
var wlwds_get_entry_number_error="获取表格数量错误!"
var wlwds_get_table_entry_error="获取表格入口错误!"
var wlwds_delete_table_entry_error="删除表格入口错误!"
var wlwds_delete_all_table_error="删除所有表格错误!"

/***********	wlactrl.htm	************/
var wlactrl_header_for_double='无线接入控制-wlan';
var wlactrl_onelan_header = '无线接入控制</p>';
var wlactrl_morelan_header = '无线接入控制 -wlan';
var wlactrl_header_explain = '如果您选择允许接入的, 只有那些MAC地址在接入控制列表中的用户才能接入到您的AP。当选择拒绝接入的，在列表中的用户不能接入AP。';
var wlactrl_accmode = '无线接入控制模式:';
var wlactrl_accmode_diable = '禁用';
var wlactrl_accmode_allowlist = '允许接入的';
var wlactrl_accmode_denylist = '拒绝接入的';
var wlactrl_macaddr = 'MAC地址';
var wlactrl_comment = '说明 ';
var wlactrl_select='选择';
var wlactrl_apply = '设置';
var wlactrl_reset = '重置';
var wlactrl_save = '保存';
var wlactrl_save_apply = '保存&生效';

var wlactrl_accctrl_list = '当前接入控制列表:';
var wlactrl_delete_select_btn = '删除选择的';
var wlactrl_delete_all_btn = '删除所有';
var wlactrl_fmwlan_macaddr = 'MAC地址';
var wlactrl_fmwlan_select = '选择';
var wlactrl_apply_explain = '如果ACL允许列表打开; WPS2.0将不起作用';
var wlactrl_apply_mac_short = '输入的MAC地址不完整，MAC地址必须是12个16进制的长度. ';
var wlactrl_apply_mac_invalid = '无效的MAC地址,MAC地址必须是16进制.';
var wlactrl_delete_result = '删除所有的入口将会导致用户不能接入AP，确定?';
var wlactrl_delete_select = '确定删除所选的入口?';
var wlactrl_delete_all = '确定删除所有的入口?';
var wlactrl_set_enable_flag="设置启用标志错误!"
var wlactrl_invalid_mac_address="错误! 无效的MAC地址."
var wlactrl_comment_length_too_long='错误! 说明长度太长.'
var wlactrl_get_entry_number='获取表格数量错误!'
var wlactrl_cannot_add_new_entry="表已满，不能添加新的表项!"
var wlactrl_rule_exist="规则已经存在!"
var wlactrl_add_table_entry_error="添加表项错误!"
var wlactrl_get_entry_number_error="获取表格数量错误!"
var wlactrl_get_table_entry_error="获取表格入口错误!"
var wlactrl_delete_table_entry_error="删除表格入口错误!"
var wlactrl_delete_all_table_error="删除所有表格错误!"

/***********	firewall	************/
var firewall_proto = '协议:';
var firewall_proto_both = 'Both';
var firewall_proto_tcp = 'TCP';
var firewall_proto_udp = 'UDP';
var firewall_add_rule = '增加规则';
var firewall_apply = '设置';
var firewall_reset = '重置';
var firewall_filtertbl = '当前过滤表:';
var firewall_delete_select = '删除所选';
var firewall_delete_all = '删除所有';

var firewall_delete_selectconfm = '确定删除所选的入口?';
var firewall_delete_allconfm = '确定删除所有的入口?';
var firewall_ipaddr_invalid = '无效的IP地址';
var firewall_port_notdecimal = '无效的端口号! 必须为0-9的十进制数.';
var firewall_port_toobig = '无效的端口号! 必须设置为1-65535的值.';
var firewall_port_rangeinvalid = '无效的端口范围! 第1个端口值必须小于第2个.';


var firewall_local_ipaddr = '本地IP地址:';
var firewall_port_range = '端口范围: ';
var firewall_comm = '注释:';
var firewall_ip_invalid_range = '无效的ip地址范围\n结束地址必须大于或者等于起始地址。';


var firewall_tbl_proto = '协议';
var firewall_tbl_comm = '注释';
var firewall_tbl_select = '选择';
var firewall_tbl_localipaddr = '本地IP地址';
var firewall_portrange = '端口范围';

/***********	portfilter.htm	************/
var portfilter_header = '端口过滤';
var portfilter_header_explain = ' 表中的条目用来限制一些从本地网络通过网关发往互联网的特定的数据包,使用这种过滤规则能够保护或者限制本地网络.';
var portfiletr_enable = '打开端口过滤';
var portfilter_noport = '必须为过滤表向设置端口范围';
var portfiletr_both = '全部';
var portfiletr_port_range = '端口范围: ';
var portfiletr_proto = '协议:';
var portfiletr_comm = '注释:';
var portfiletr_filtertbl = '当前过滤表:';
var portfiletr_save = '保存';
var portfiletr_apply = '保存&生效';
var portfiletr_reset = '重置';
var portfilter_delete = '删除选中项';
var portfilter_delete_all = '删除所有';
var portfilter_port_invalid = '无效的端口号! 必须为十进制数(0-9).';
var portfilter_port_invalid_num = '无效的端口号! 必须设置端口号在1-65535之中.';
var portfilter_port_invalid_range = '无效的端口范围! 第一个端口号必须小于第二个端口号.';
var portfilter_delete_selectconfm = '确定删除所选的入口?';
var portfilter_delete_allconfm = '确定删除所有的入口?';
var portfiletr_tbl_range = '端口范围';
var portfiletr_tbl_protocol = '协议';
var portfiletr_tbl_comm = '注释';
var portfiletr_tbl_select = '选择';
var portfilter_Enable_IPv4 = '打开IPv4';
var portfilter_Enable_IPv6 = '打开IPv6';
var portfiletr_tbl_ipversion = 'IP版本';
var portfiletr_overlap = '端口重叠!';


/***********	ipfilter.htm	************/
var ipfilter_header = 'IP过滤';
var ipfilter_header_explain = ' 表中的条目用来限制一些从本地网络通过网关发往互联网的特定的数据包,使用这种过滤规则能够保护或者限制本地网络.';
var ipfilter_enable = '打开IP过滤';
var ipfilter_ipv4_enable = '打开IPv4';
var ipfilter_ipv6_enable = '打开IPv6';
var ipfilter_local_ipv4_addr = '源IPv4地址: ';
var ipfilter_dst_ipv4_addr = '目的IPv4地址: ';
var ipfilter_local_ipv6_addr = '源IPv6地址: ';
var ipfilter_mode = 'IP过滤模式: ';
var ipfilter_mode_black = '黑名单';
var ipfilter_mode_white = '白名单';
var ipfilter_allwan = '所有WAN口';
var ipfilter_proto = '协议:';
var ipfilter_comm = '注释:';
var ipfilter_save = '保存';
var ipfilter_apply = '保存&生效';
var ipfilter_reset = '重置';
var ipfilter_filtertbl = '当前过滤表:';
var ipfilter_tbl_local_addr = '源IP起始地址';
var ipfilter_tbl_local_end_addr = '源IP结束地址';
var ipfilter_tbl_dst_addr = '目的IP起始地址';
var ipfilter_tbl_dst_end_addr = '目的IP结束地址';
var ipfilter_tbl_proto = '协议';
var ipfilter_tbl_wan = 'Wan口';
var ipfilter_tbl_comm = '注释';
var ipfilter_tbl_select = '选择';
var ipfilter_delete = '删除选中项';
var ipfilter_delete_all = '删除所有';
var ipfilter_delete_selectconfm = '确定删除所选的入口?';
var ipfilter_delete_allconfm = '确定删除所有的入口?';
var ipfilter_change_to_blacklist = '确定改变过滤模式为黑名单?';
var ipfilter_change_to_whitelist = '确定改变过滤模式为白名单?';
var ipfilter_ipaddr_invalid = '无效的IP地址';
var ipfilter_ip6addr_invalid = '无效的IPv6地址!';
var ipfilter_set_error1 = '请选择IP转发WAN口.';
var ipfilter_type_ip = '基于IP范围';
var ipfilter_type_proto = '基于协议';
var ipfilter_type = 'IP过滤类型:';
var ipfilter_rule_exist = '规则已经存在!';
var ipfilter_comm_oversize = '错误! 注释长度太长.';

var ipfilter_ip_overlap = 'ip地址重叠!';
var ipfilter_ipv6_rule_exist = 'ipv6规则已经存在!';
var ipfilter_set_error1 = '请选择IP转发WAN口.';
var ipfilter_set_mode_error = '设置过滤模式错误';
var ipfilter_no_ipaddr = '错误! 没有设置IP地址.';
var ipfilter_ipaddr_not_within_subnet = '无效的IP地址!必须设在当前子网下.';
var ipfilter_miss_ipaddr = '错误！IP地址设置不完整.';
var ipfilter_type_error = '错误! 协议类型不匹配.';
var ipfilter_invalid_proto = '错误！协议类型不支持.';
var ipfilter_proto_empty = '错误！协议类型不能为空.';

/***********	Macfilter.htm	************/
var macfilter_header = 'MAC过滤';
var macfilter_header_explain =  '表中的条目用来限制一些从本地网络通过网关发往互联网的特定的数据包,使用这种过滤规则能够保护或者限制本地网络.';
var macfilter_enable = '打开MAC过滤';
var macfilter_macaddr = 'MAC地址: ';
var macfilter_smacaddr = '源MAC地址: ';
var macfilter_dmacaddr = '目的MAC地址: ';
var macfilter_smac = '源MAC地址';
var macfilter_dmac = '目的MAC地址';
var macfilter_comment = '注释';
var macfilter_select = '选择';
var macfilter_comm = '注释: ';
var macfilter_macaddr_invalid = 'mac地址无效!';
var macfilter_macaddr_nocomplete = '输入的MAC地址不完整，MAC地址必须是12位16进制';
var macfilter_macaddr_nohex = '无效的MAC地址,MAC地址必须为16进制的数字.';
var macfilter_macaddr_nozero = '无效的MAC地址.MAC地址不能为00:00:00:00:00:00.';
var macfilter_macaddr_nobroadcast = '无效的MAC地址.MAC地址不能为ff:ff:ff:ff:ff:ff.';
var macfilter_macaddr_nomulticast = '无效的MAC地址.MAC地址不能为01:00:5e:00:00:00到01:00:5e:7f:ff:ff之间的组播地址.';
var macfilter_filterlist_macaddr = 'MAC地址';
var macfilter_save = '保存';
var macfilter_apply = '保存&生效';
var macfilter_reset = '重置';
var macfilter_current_table = '当前过滤表:';
var macfilter_delete = '删除选中项';
var macfilter_delete_all = '删除所有';
var macfilter_delete_confirm = '确定删除所选的入口?';
var macfilter_delete_all_confirm = '确定删除所有的入口?';
var macfilter_blacklist = '禁止mac地址(黑名单)';
var macfilter_whitelist = '允许mac地址(白名单)';
var macfilter_whitelist_add_alert = '请不要忘记至少添加一个白名单';




/***********	Portfw.htm	************/
var portfw_header = '端口转发';
var portfw_header_explain = '表中的条目允许你将一些公共网络服务设置在NAT防火墙后一个内网设备.只有当你希望在私有网络设置类似web或mail服务器时，才需要进行配置.';
var portfw_enable = '打开端口转发';
var portfw_ipaddr = 'IP地址:';
var portfw_apply_port_empty = '端口范围不能为空! 必须将端口的值设置为1-65535.';
var portfw_tbl = '当前的端口转发表:';
var portfw_delete_confirm = '确定删除所选的入口?';
var portfw_delete_all_confirm = '确定删除所有的入口?';
var portfw_all_wan = '所有WAN口:';
var portfw_local_ipaddr = '本地IP地址:';
var portfw_local_portrange = '本地端口范围:';
var portfw_proto = '协议:';
var portfw_both = '全选';
var portfw_remote_ipaddr = '广域网IP地址:';
var portfw_remote_portrange = '广域网端口范围:';
var portfw_comm = '注释:';
var portfw_save = '保存';
var portfw_apply = '保存&生效';
var portfw_reset = '重置';
var portfw_delete = '删除选中项';
var portfw_delete_all = '删除所有';
var portfw_port_invalid1 = '无效端口号! 它应该为十进制数(0-9).';
var portfw_port_invalid2 = '无效端口号! 端口号指必须在范围1-65535中.';
var portfw_port_invalid3 = '无效端口范围! 第一个端口值必须小于第二个端口值.';
var portfw_externel_port_empty = '远端端口范围不能为空! 必须将端口的值设置为1-65535.';
var portfw_externel_port_invalid1 = '无效远端端口号! 它应该为十进制数(0-9).';
var portfw_externel_port_invalid2 = '无效远端端口号! 端口号指必须在范围1-65535中.';
var portfw_externel_port_invalid3 = '无效远端端口范围! 第一个端口值必须小于第二个端口值.';
var portfw_tbl_local_ipaddr = '本地IP地址';
var portfw_tbl_local_portrange = '本地端口范围';
var portfw_tbl_proto = '协议';
var portfw_tbl_remote_ipaddr = '广域网IP地址';
var portfw_tbl_remote_portrange = '广域网端口范围';
var portfw_tbl_wan = 'Wan口';
var portfw_tbl_status = '状态';
var portfw_tbl_remote_comm = '注释';
var portfw_tbl_select = '选择';
var portfw_tbl_portrange = '端口范围';
var portfw_ipaddr_invalid = '无效IP地址';
var portfw_tbl_any = '任意';

var portfw_wan_not_select = '请选择端口转发的wan口.';
var portfw_ip_error = '无效的IP地址! 应该设置在当前子网下.';
var portfw_range_overlapped = '设置的端口范围与使用的端口值重叠!';
var portfw_ip_error2 = '无效的IP地址! 不能设置在当前lan子网下.';
var portfw_table_full = '不能加入新规则，表中已满!';
var portfw_no_ipaddr = '错误! 没有设置ip地址.';
var portfw_enable = '开启';
var portfw_disable = '关闭';



/***********	urlfilter.htm	************/
var urlfilter_header = 'URL过滤';
var urlfilter_header_explain = ' URL过滤用来拒绝LAN用户访问互联网的请求,阻止用户访问那些包含关键字的URL.';
var urlfilter_enable = '打开URL过滤';
var urlfilter_urladdr = 'URL地址: ';
var urlfilter_apply_error = '错误符号: \";\"';
var urlfilter_filterlist_yrladdr = 'URL地址';
var urlfilter_blacklist = '禁止url地址(黑名单)';
var urlfilter_whitelist = '打开url地址(白名单)';
var urlfilter_save = '保存';
var urlfilter_apply = '保存&生效';
var urlfilter_reset = '重置';
var urlfilter_current_table = '当前过滤表:';
var urlfilter_delete = '删除选中项';
var urlfilter_delete_all = '删除所有';
var urlfilter_delete_confirm = '确定删除所选的入口?';
var urlfilter_delete_all_confirm = '确定删除所有的入口?';
var urlfilter_select = '选择';
var urlfilter_ipaddr = 'IP地址';
var urlfilter_macaddr = 'Mac地址';
var urlfilter_all_users = '所有用户 (默认规则)';
var urlfilter_mode = '用户模式: ';
var urlfilter_entry_full = '不能添加新URL规则，条目已满!';



/***********	algonoff.asp	************/
var algonoff_enable_L2TP = '启用 L2TP ALG';
var algonoff_enable_IPSec = '启用 IPSec ALG';
var algonoff_enable_H323 = '启用 H.323 ALG';
var algonoff_enable_RSTP = '启用 RSTP ALG';
var algonoff_enable_SIP = '启用 SIP ALG';
var algonoff_enable_FTP = '启用 FTP ALG';
var algonoff_enable_PPTP = '启用 PPTP ALG';
var algonoff_apply = '保存/应用';


/***********	dmz.htm	************/
var dmz_header = 'DMZ';
var dmz_header_explain = 'DMZ可以在不牺牲内网的非授权访问的特性来提供互联网服务.典型的,DMZ主机包含一些必须被互联网公开访问的设备,例如Web(HTTP)服务器,FTP服务器,SMTP(e-mail)服务器和DNS服务器.';
var dmz_enable = '打开DMZ';
var dmz_host_ipaddr = 'DMZ主机IP地址:';
var dmz_ip_invalid = '无效的IP地址';
var dmz_checkip1 = 'IP地址不能为空! IP地址为如 xxx.xxx.xxx.xxx所示的4个十进制数.';
var dmz_checkip2 = '值. 它必须为十进制数(0-9).';
var dmz_checkip3 = '范围在第一个字节中. 它必须在1-223之间.';
var dmz_checkip4 = '范围在第一个字节中. 它不能为127.';
var dmz_checkip5 = '范围在第二个字节中. 它必须在0-255之间.';
var dmz_checkip6 = '范围在第三个字节中. 它必须在0-255之间.';
var dmz_checkip7 = '范围在第四个字节中. 它必须在1-254之间.';
var dmz_save = '保存';
var dmz_apply = '保存&生效';
var dmz_reset = '重置';

var dmz_ip_error = '无效的ip地址! 应该设置在当前子网下.';
var dmz_set_mib_error = '设置 DMZ MIB 错误!';

var dmz_ip_error2 = '无效的ip地址! 它不能等于局域网IP地址.';


/***********	app_upnp_cmcc.asp	************/
var app_upnp_cmcc_enable = '启用UPnP:';
var app_upnp_cmcc_yes = '确认';
var app_upnp_cmcc_cancel = '取消';

/***********	snooping_proxy_cmcc.asp	************/
var cmcc_enable_snooping = '启用IGMP/MLD Snooping:';
var cmcc_enable_proxy = '启用IGMP/MLD Proxy:';
var snooping_proxy_cmcc_apply = '应用';


/***********	logout.htm	************/
var logout_header = '注销';
var logout_header_explain = '这个页面用来退出系统.';
var logout_confm = '确定要退出系统 ?';
var logout_apply = '设置';

/***********	samba.asp	************/
var app_samba_enable = '启用Samba:';

/***********	minidlna.asp	************/
var app_minidlna_enable = '启用DLNA:';

/***********	password.htm	************/
var password_header = '密码设置';
var password_header_explain = '这个页面用来设置帐号访问AP的web服务器,空的用户名和密码将会取消这种保护.';
var password_user_name = '用户名:';
var password_user_passwd = '新密码:';
var password_user_passwd_confm = '确认密码:';
var password_apply = '设置';
var password_user_empty = '用户帐号是空的.\n取消密码保护?';
var password_passwd_unmatched = '密码不匹配,请在新密码栏和确认密码栏中重新输入相同的密码';
var password_passwd_empty = '密码不能为空,请重新输入.';
var password_user_invalid = '用户名不接受空格符,请重新输入.';
var password_passwd_invalid = '密码不接受空格符,请重新输入.';
var password_reset = '  重置  ';
var password_save = '保存';
var password_save_apply = '保存&设置';
var password_change_setting = '成功改变设置!';


/***********	super_password.htm	************/
var super_password_empty = ' 不能为空!';
var super_password_space = '不能接受空格符 ';
var super_password_try = ',请重新输入.';
var super_password_superpasswd_unmatched =  '密码不匹配,请在超级用户下的新密码栏和确认密码栏中重新输入相同的密码';
var super_password_passwd_unmatched = '密码不匹配,请在新密码栏和确认密码栏中重新输入相同的密码';
var super_password_header = '修改密码';
var super_password_header_explain = '这个页面用来设置帐号访问AP的web服务器,空的用户名和密码将会取消这种保护.';
var super_password_superusername = '超级用户名:';
var super_password_username = '用户名:';
var super_password_user_passwd = '新密码:';
var super_password_user_passwd_confm = '确认密码:';
var super_password_admin_user = '管理员用户';
var super_password_apply = '设置';
var super_password_reset = ' 重置 ';

/***********	logout.htm	************/
var logout_header = '退出登录';
var logout_header_explain = '这个网页用来退出登录.';
var logout_want_logout = '是否退出登录 ?';
var logout_apply = '退出';

/***********	diagnostic.htm	************/
var diagnostic_hearder = '诊断';
var diagnostic_hearder_explain = '这个页面被用来诊断系统问题，诊断结果将保存在txt文档里.';
var diagnostic_status_overview = ' 状态概览: ';
var diagnostic_current_status = '获取当前系统状态概览';
var diagnostic_wireless_related = ' 无线相关: ';
var diagnostic_wlan_diagnostic = '当wlan不正常时，获取wlan的相关诊断';
var diagnostic_phenomenon ='现象：';
var diagnostic_scan = '扫描失败';
var diagnostic_connect = '连接失败';
var diagnostic_throughput = '低吞吐量';
var diagnostic_analyze = '分析';
var openvpn_header = 'OpenVPN ; 设置';
var openvpn_header_explain = '这个页面被用来配置OpenVPN的参数 ';
var openvpn_enable = '开启 OpenVPN ';
var openvpn_mode = ' 模式 : ';
var openvpn_server = ' 服务器 ';
var openvpn_port = '端口 : ';
var openvpn_port_empty = 'OpenVPN端口不能为空';
var openvpn_type = ' 授权类型 : ';
var openvpn_username_password = '用户名/密码 ';
var openvpn_certification = '认证 ';
var openvpn_username_password_certification = '用户名/密码+认证 ';
var openvpn_apply = '设置';
var openvpn_reset = ' 重置 ';
var openvpn_save = '保存...';
var openvpn_save_config = '保存客户端配置文件:';

/***********	zigbee.htm	************/
var zigbee_header = 'ZigBee 管理';
var zigbee_header_explain = '这个表格显示当前匹配的的ZigBee设备.';
var zigbee_address = 'IEEE 地址';
var zigbee_status = '状态';
var zigbee_brightness = '亮度';
var zigbee_refresh = '刷新';
var zigbee_apply = '设置';
var zigbee_lightoff = '关灭';
var zigbee_lighton = '点亮';
var zigbee_set_brightness = '设置亮度';
var zigbee_join = '允许加入';

/***********	weave.htm	************/
var weave_invalid_registration = '非法的注册码. 注册号应该是16进制数像 f840e274-ce08-8c5f-2ef8-ebdab3bab1c3.';
var weave_input_null = '输入不能为空！请重新输入或者取消！';
var weave_clean_device = '清除保留的设备注册信息？确认的话你需要重新注册 请输入密码';
var weave_input_password = '请输入密码!';
var weave_invalid_commit = '非法的提交值!没有被初始化!';
var weave_config = 'Weave 配置';
var weave_register = 'Weave 注册';
var weave_registered = '已注册';
var weave_unregistered = '未注册';
var weave_register_ticket = '注册码';
var weave_summit_register = '注册';

/***********	tramsmission.htm	************/
var tramsmission_header = 'BT 控制器';
var tramsmission_header_explain = '启动/停止 BT. 请用远端传输获取更多信息.';
var tramsmission_bt_enable = '启动 BT';
var tramsmission_bt_download = 'BT 下载目录';
var tramsmission_save = '保存';

/***********	cpuShow.htm	************/
var cpu_wait = '请等待10秒!';
var cpu_current = '当前:';
var cpu_peak = '峰值:';
var cpu_start_time = '起始时间: ';
var cpu_get_history = '不能获取到历史数据!';
var cpu_get_future = '不能获取到未来数据!';
var cpu_set_start = '设置起始时间错误!';
var cpu_time_between = '时间范围必须在 ';
var cpu_and = ' 和 ';
var cpu_enable = '开启 CPU 应用';
var cpu_set_start_time = '设置起始时间 :';
var cpu_time_scale = '时间刻度 :';
var cpu_statistics = '统计 :';
var cpu_apply = '设置';
var cpu_refresh = '刷新';



/***********	saveconf.htm	************/
var saveconf_header = '保存/重新加载配置';
var saveconf_header_explain = '这个页面允许你保存当前的配置到文件中,也可以从文件中加载新的配置.除此之外,你还可以将当前配置恢复成默认配置.';
var saveconf_save_to_file = '保存配置到文件:';
var saveconf_save = '保存...';
var saveconf_load_from_file = '加载配置从文件:';
var saveconf_load = '上传';
var saveconf_reset_to_default = '恢复默认配置:';
var saveconf_reset = '重置';
var saveconf_load_from_file_empty = '请选择一个文件!';
var saveconf_reset_to_default_confm = '确认将当前设置恢复成默认配置?';
var saveconf_select_file = '请选择一个文件！';
var saveconf_reload_setting = '成功加载设置!';
var saveconf_router_booting = '路由器正在重启.';
var saveconf_aprouter_booting = 'AP正在重启.';
var saveconf_not_turnoff_reboot = '请不要在此期间关闭或重启设备.';
var saveconf_buffer_failed = '分配缓冲区失败!';
var saveconf_current_setting_failed = 'Write default to current setting failed!\n';
var saveconf_invalid = '非法的配置文件!';


/***********	upload.htm	************/
var upload_header = '升级固件';
var upload_header_explain = '这个页面允许你更新AP的固件,请注意,在上传的过程中设备如果断电可能会使系统崩溃.';
var upload_explain = '这个页面允许你更新AP的固件(例如，/tmp/usb/sda1/fw.bin),请注意,在上传的过程中设备如果断电可能会使系统崩溃.';
var upload_version = '固件版本:';
var upload_file_select = '选择文件:';
var upload_send = '上传';
var upload_reset = '重置';
var upload_up_failed = '更新固件失败!';
var upload_send_file_empty = '文件名不能为空 !';
var upload_usb_update_not_enable = 'usb更新镜像函数没有被开启！';
var upload_cannot_upgrade = '不能通过web UI升级固件！ ACS 已经设置.管理服务器.升级管理正确';
var upload_invalid_format = '非法的文件格式！';
var upload_dual_firmware = '开启双固件';
var upload_wait = '请等待...';


/***********	syslog.htm	************/
var syslog_header = '系统日志';
var syslog_header_explain = '这个页面被用来设置远程日志服务器和现实系统日志.';
var syslog_enable = '打开日志';
var syslog_sys_enable = ' 整个系统 ';
var syslog_wireless_enable = '无线网络';
var syslog_dos_enable = '拒绝服务';
var syslog_11s_enable = '11s';
var syslog_rtlog_enable = '打开远程日志';
var syslog_local_ipaddr = '日志服务器IP地址:';
var syslog_apply = '设置';
var syslog_refresh = '刷新';
var syslog_clear = ' 清除 ';
var syslog_invalid_server_address = '非法的服务器地址！';


/***********	dos.htm	************/
var dos_header = '拒绝服务';
var dos_header_explain = '拒绝服务攻击是由黑客发动的攻击,以阻止合法用户访问互联服务.';
var dos_enable = '启用防止拒绝服务';

var dos_packet_sec = ' 包/秒';
var dos_sysflood_syn = '整个系统泛洪: SYN';
var dos_sysflood_fin = '整个系统泛洪: FIN';
var dos_sysflood_udp = '整个系统泛洪: UDP';
var dos_sysflood_icmp = '整个系统泛洪: ICMP';
var dos_ipflood_syn = '每个源口IP泛洪: SYN';
var dos_ipflood_fin = '每个源口IP泛洪: FIN';
var dos_ipflood_udp = '每个源口IP泛洪: UDP';
var dos_ipflood_icmp = '每个源口IP泛洪: ICMP';
var dos_portscan = 'TCP/UDP端口扫描';
var dos_portscan_low = '低';
var dos_portscan_high = '高';
var dos_portscan_sensitivity = '敏感性';
var dos_icmp_smurf = 'ICMP Smurf';
var dos_ip_land = 'IP Land';
var dos_ip_spoof = 'IP Spoof';
var dos_ip_teardrop = 'IP TearDrop';
var dos_pingofdeath = 'PingOfDeath';
var dos_tcp_scan = 'TCP扫描';
var dos_tcp_synwithdata = 'TCP SynWithData';
var dos_udp_bomb = 'UDP Bomb';
var dos_udp_echochargen = 'UDP EchoChargen';
var dos_select_all = ' 全部选择 ';
var dos_clear_all = '全部清除';
var dos_enable_srcipblocking = '启动源IP阻止';
var dos_block_time = '阻止时间(秒)';
var dos_apply = '保存&生效';
var dos_enable_ipv6 = '启用ipv6拒绝服务';
var dos_ipv6_smurf= 'Ipv6 Smurf';
var dos_ipv6_icmp_big= 'Ipv6 Icmp过大';
var dos_ipv6_Flood= 'Ipv6 Ns泛洪';
var dos_icmp_redirection= 'ICMP 重定向';
var dos_winnuke= 'Winnuke';
var dos_change_setting_success='成功改变设置!';


/***********	snmp.htm	************/
var snmp_header = 'SNMP&nbsp; 设置';
var snmp_header_explain = 'SNMP是用于网络管理的应用';
var snmp_enable = '打开SNMP ';
var snmp_name = '名称 : ';
var snmp_location = '位置 :';
var snmp_contanct = '接触 :';
var snmp_rw_conmmunity = '读/写 Conmmunity : ';
var snmp_r_only_conmmunity = '只读 Community : ';
var snmp_save = '保存';
var snmp_apply = '保存&生效';
var snmp_reset = '重置';
var snmp_name_empty = 'SNMP名称不能为空';
var snmp_location_empty = 'SNMP位置不能为空';
var snmp_contanct_empty = 'SNMP接触不能为空';
var snmp_rw_conmmunity_empty = '读写Community不能为空 ';
var snmp_r_only_conmmunity_empty = '只读Community不能为空 ';



/***********	ntp.htm	************/
var ntp_header = '时区设置';
var ntp_header_explain = '你可以通过与Internet上的时间服务器进行同步来维护系统时间.';
var ntp_curr_time = ' 当前时间 : ';
var ntp_year = '年';
var ntp_month = '月';
var ntp_day = '日';
var ntp_hour = '小时';
var ntp_minute = '分';
var ntp_second = '秒';
var ntp_copy_comptime = '复制计算机时间';
var ntp_time_zone = '时区选择 : ';
var ntp_enable_clientup = '启动NTP客户端更新 ';
var ntp_adjust_daylight = '自动夏令时调整 ';
//var ntp_server = ' SNTP服务器 : ';
var ntp_server = ' NTP服务器 : ';
var ntp_server_north_america1 = '198.123.30.132    - 北美';
var ntp_server_north_america2 = '209.249.181.22   - 北美';
//var ntp_server_Europe1 = '85.12.35.12  - 欧洲';
//var ntp_server_Europe2 = '217.144.143.91   - 欧洲';
var ntp_server_Europe1 = '131.188.3.220  - 欧洲';
var ntp_server_Europe2 = '130.149.17.8   - 欧洲';
var ntp_server_Australia = '223.27.18.137  - 澳洲';
//var ntp_server_asia1 = '133.100.11.8 - 亚太区';
var ntp_server_asia1 = '203.117.180.36 - 亚太区';
var ntp_server_asia2 = '210.72.145.44 - 亚太区';
var ntp_manu_ipset = ' (手动设置IP) ';
var ntp_save = '保存';
var ntp_save_apply = '保存&生效';

var ntp_apply = '设置';
var ntp_reset = '重置';
var ntp_refresh = '刷新';
var ntp_month_invalid = '无效的月份,这些值必须由0-9的数字组成.';
var ntp_time_invalid = '无效的时间!';
var ntp_ip_invalid = '无效的IP地址';
var ntp_servip_invalid = '无效的NTP服务器IP地址!这个值不能为空.';
var ntp_field_check = ' 域不能为空\n';
var ntp_invalid = '无效的 ';
var ntp_num_check = ' 值,这些值必须由0-9的数字组成.';

/***********	ddns.htm	************/
var ddns_header = '动态域名服务器设置';
var ddns_header_explain = '动态域名服务为你提供一个可变IP地址和有效且不变的互联网域名(URL)的之间的配对.';
var ddns_enable = '启动DDNS ';
var ddns_serv_provider = ' 服务提供商 : ';
var ddns_dyndns = 'DynDNS ';
var ddns_orayddns = 'OrayDDNS ';
var ddns_orayddns = 'TZO ';
var ddns_domain_name = '域名 : ';
var ddns_user_name = ' 用户名/邮箱: ';
var ddns_passwd = ' 密码/密匙: ';
var ddns_note = '注解:';
var ddns_oray_header = '对于Oray DDNS,你可以创建自己的Oray帐号';
var ddns_here = '这里 ';
var ddns_dyn_header = '对于DynDNS,你可以创建自己的DynDNS帐号';
var ddns_tzo_header1 = '对于TZO，你可以获得30天免费试';
var ddns_tzo_header2 = '或在这里管理你的帐号';
var ddns_tzo_header3 = '管理';
var ddns_apply = '设置';
var ddns_reset = '重置';
var ddns_domain_name_empty = '域名不能为空';
var ddns_user_name_empty = '用户名/邮箱不能为空';
var ddns_passwd_empty = '密码/密匙不能为空';
var ddns_save = '保存';
var ddns_save_apply = '保存&生效';


/***********	ip_qos.htm	************/
var ip_qos_header = 'QoS';
var ip_qos_header_explain = ' 表中的条目用于改善你的在线游戏体验,确保你的游戏流量的优先级高于其他网络流量.';
var ip_qos_enable = '启动QoS';
var ip_qos_bandwidth = '带宽限速';
var ip_qos_schedule = '加权队列调度';
var ip_qos_auto_upspeed = '自动上行速度';
var ip_qos_manu_upspeed = '手动上行速度(Kbps):';
var ip_qos_auto_downspeed = '自动下行速度';
var ip_qos_manu_downspeed = '手动下行速度(Kbps):';
var ip_qos_rule_set = 'QoS规则设定:';
var ip_qos_name = '名称:';
var ip_qos_type = 'QoS类型:';
var ip_qos_ipvertion = 'ip版本:';
var ip_qos_addrtype = '地址类型:';
var ip_qos_addrtype_ip = 'IP';
var ip_qos_addrtype_mac = 'MAC';
var ip_qos_local_ipaddr = '本地IP地址:';
var ip_qos_proto = '协议:';
var ip_qos_proto_udp = 'udp';
var ip_qos_proto_tcp = 'tcp';
var ip_qos_proto_both = '全选';
var ip_qos_local_port = '本地端口:(1~65535)';
var ip_qos_remote_ipaddr = '远端IP地址:';
var ip_qos_remote_port = '远端端口:(1~65535)';
var ip_qos_macaddr = 'MAC地址:';
var ip_qos_ipv6addr = 'IPv6地址:';
var ip_qos_remote_macaddr = '远端MAC地址:';
var ip_qos_phyport = '物理端口:';
var ip_qos_mode = '模式:';
var ip_qos_weight = '权重';
var ip_qos_upband = '上行带宽(Kbps):';
var ip_qos_downband = '下行带宽(Kbps):';
var ip_qos_policy = '策略:';
var ip_qos_priority = '优先级:';
var ip_qos_weight = '权重:';
var ip_qos_comm = '注释:';
var ip_qos_apply = '设置';
var ip_qos_save = '保存';
var ip_qos_save_apply = '保存&生效';
var ip_qos_reset = '重置';
var ip_qos_curr_qos = '当前QoS规则表:';
var ip_qos_delete_select_btn = '删除所选';
var ip_qos_delete_all_btn = '删除所有';
var ip_qos_remark = '备注';
var ip_qos_dscp_remark = '备注dscp:';


var ip_qos_upspeed_empty = '自动上行速度关闭时，手动上行速度不能为空且不能比100小.';
var ip_qos_downspeed_empty = '自动下行速度关闭时，手动下行速度不能为空且不能比100小.';
var ip_qos_ip_invalid = '无效的IP地址';
var ip_qos_remote_ip_invalid = '无效的远端IP地址';
var ip_qos_ip_range_invalid = '无效的IP地址范围!\n结束地址必须大于起始地址.';
var ip_qos_remote_ip_range_invalid = '无效的远端IP地址范围!\n结束地址必须大于起始地址.';
var ip_qos_startip_invalid = '无效的起始IP地址,IP地址必须在当前子网的范围内.';
var ip_qos_endip_invalid = '无效的结束IP地址,IP地址必须在当前子网的范围内.';
var ip_qos_portrange_invalid = '无效的端口范围,端口的范围为1-65535.';
var ip_qos_port_invalid = '无效的端口!';
var ip_qos_mac_invalid = 'mac地址无效!';
var ip_qos_macaddr_notcomplete = '输入的MAC地址不完整,MAC地址必须为12个16进制长度';
var ip_qos_remote_macaddr_notcomplete = '输入的远端MAC地址不完整,MAC地址必须为12个16进制长度';
var ip_qos_macaddr_invalid = '无效的MAC地址,MAC地址由16进制数组成(0-9或a-f)且不能为全零.';
var ip_qos_band_empty = '上行带宽或下行带宽不能为0或为空.';
var ip_qos_band_invalid = '无效的输入!必须为0-9的十进制数.';
var ip_qos_band_notint = '无效的输入! 必须为整数。';
var ip_qos_weight_empty = '权重值不能为空.';
var ip_qos_weight_invalid = '无效的权重范围!权重的范围为1-20的整数.';
var ip_qos_priority_empty = '优先级不能为空.';
var ip_qos_priority_invalid = '无效的优先级范围!优先级的范围为1~8的整数.';
var ip_qos_macaddr_invalid = 'mac地址无效!';
var ip_qos_remote_macaddr_invalid = '远端mac地址无效!';
var ip_qos_delete_select = '确定删除所选表项?';
var ip_qos_delete_all = '确定删除所有表项?';
var ip_qos_mac_input_error = '不能同时输入本地和远端mac地址!';
var ip_qos_ipport_input_error = '不能同时输入本地和远端ip地址/端口!';
var ip_qos_num_invalid = '无效的数字!';
var ip_qos_down_phyport_invalid = '无效的下行链路物理端口!';
var ip_qos_up_phyport_invalid = '无效的上行链路物理端口!';
var ip_qos_ipv6_length_invalid = '错误的ipv6地址长度!';
var ip_qos_ipv6_invalid = 'IP6地址无效!';

var ip_qos_tbl_name = '名称';
var ip_qos_tbl_ipversion = 'Ip版本';
var ip_qos_tbl_proto = '协议';
var ip_qos_tbl_localport = '本地端口';
var ip_qos_tbl_remote_port = '远端端口';
var ip_qos_tbl_remote_addr = '远端IP地址';
var ip_qos_tbl_local_ipv6addr = '本地IPv6地址';
var ip_qos_tbl_remote_macaddr = '远端MAC';
var ip_qos_tbl_Phyport = '物理端口';
var ip_qos_tbl_priority = '优先级';
var ip_qos_tbl_weight = '权重';
var ip_qos_tbl_dscp_remark = '备注dscp';
var ip_qos_tbl_comm = '备注';

var ip_qos_tbl_localaddr = '本地IP地址';
var ip_qos_tbl_macaddr = 'MAC地址';
var ip_qos_tbl_mode = '模式';
var ip_qos_tbl_policy = '策略';
var ip_qos_tbl_valid ='有效';
var ip_qos_tbl_upband = '上行带宽';
var ip_qos_tbl_downband = '下行带宽';
var ip_qos_tbl_select = '选择';
var ip_qos_restrict_maxband = "限制最大带宽";
var ip_qos_quarant_minband = "保护最小带宽";

var ip_qos_up = "上行";
var ip_qos_down = "下行";

var ip_qos_without_bandwith = "不能在不设置带宽的情况下设置qos规则!";
var ip_qos_with_nothing = "不能在不设置其它选项的情况下设置qos规则!";
var ip_qos_BOTHLINK_BW_OVERSIZE = "错误: 为了保证上行链路和下行链路的最小带宽, 所有qos规则的带宽和比总带宽大!";
var ip_qos_DOWNLINK_BW_OVERSIZE = "错误: 为了保证下行链路的最小带宽, 所有qos规则的带宽和比总下行链路带宽大!";
var ip_qos_UPLINK_BW_OVERSIZE = "错误: 为了保证上行链路的最小带宽, 所有qos规则的带宽和比总上行链路带宽大!";

var ip_qos_conflict = "本地ip地址或端口冲突!";
var ip_qos_ip_conflict = "本地ip冲突!";
var ip_qos_port_conflict = "本地端口冲突!";
var ip_qos_remote_conflict = "远端ip地址或端口冲突!";
var ip_qos_remote_ip_conflict = "远端ip冲突!";
var ip_qos_remote_port_conflict = "远端port冲突!";
var ip_qos_remote_mac_conflict = "远端mac地址冲突!";
var ip_qos_mac_conflict = "mac地址冲突!";
var ip_qos_phy_conflict = "物理端口冲突!";
var ip_qos_dscp_conflict = "DSCP冲突!";
var ip_qos_1p_conflict = "1P冲突!";
var ip_qos_entry_conflict = "条目名冲突!";
var ip_qos_tlb_enable = "开启";
var ip_qos_tlb_disable = "关闭";



/***********	traffic_shaping.htm	************/
var traffic_shaping_header = 'QoS流量整形';
var traffic_shaping_header_explain = ' 下列表项是用于配置wan口上的流量整形.';
var traffic_shaping_enable = '启动QoS';
var traffic_shaping_auto_upspeed = '自动上行速度';
var traffic_shaping_manu_upspeed = '手动上行速度(Kbps):';
var traffic_shaping_auto_downspeed = '自动下行速度';
var traffic_shaping_manu_downspeed = '手动下行速度(Kbps):';
var traffic_shaping_wan_setting = 'WAN接口QoS设置:';
var traffic_shaping_interface = '接口:';
var traffic_shaping_no_available = '无可用接口';
var traffic_shaping_enable_ipv4_qos = '启用Ipv4 QoS:';
var traffic_shaping_enable_ipv6_qos = '启用Ipv6 QoS:';
var traffic_shaping_apply = '设置';
var traffic_shaping_save = '保存';
var traffic_shaping_save_apply = '保存&生效';
var traffic_shaping_reset = '重置';

var traffic_shaping_current_table = '当前QoS流量整形表:';
var traffic_shaping_delete = '删除选中项';
var traffic_shaping_delete_all = '删除所有';

var traffic_shaping_wan_empty = 'wan口不能为空!';

var traffic_shaping_tbl_interface = '接口';
var traffic_shaping_tbl_ipv4_status = 'Ipv4状态';
var traffic_shaping_tbl_ipv6_status = 'Ipv6状态';
var traffic_shaping_tbl_uplink = '上行链路(kbps)';
var traffic_shaping_tbl_downlink = '下行链路(kbps)';
var traffic_shaping_tbl_select = '选择';

var traffic_enable = '开启';
var traffic_disable = '关闭';

var traffic_uplink_exceed = '手动上行链路超过最大范围!';
var traffic_downlink_exceed = '手动下行链路超过最大范围!';
var traffic_uplink_less_queue_shaping = '手动上行链路比队列整形小, 请先移除队列设置!';
var traffic_downlink_less_queue_shaping = '手动下行链路比队列整形小, 请先移除队列设置!';



/***********	qos_policy.htm	************/
var qos_policy_header = 'QoS策略';
var qos_policy_header_explain = '该页面用于配置qos策略和队列的设置。如果选择PRIO，则更小的值代表更大的优先级。如果选择WRR，则更大的值代表更多的带宽。';
var qos_policy_setting = 'QoS策略设置:';
var qos_policy_Direction = '方向:';
var qos_policy_uplink = '上行';
var qos_policy_downlink = '下行';
var qos_policy_Interface = '接口:';
var qos_policy_transmission = '传输:';
var qos_policy_queue = '队列';
var qos_policy_no_queue = '无可用队列';
var qos_policy_shape_mode = '整形模式:';
var qos_policy_shape_bandwidth = '整形带宽(Kbps):';
var qos_policy_default_queue = '默认队列:';
var qos_policy_table = '当前QoS策略表:';
var qos_policy_apply = '设置';
var qos_policy_save = '保存';
var qos_policy_save_apply = '保存&生效';
var qos_policy_reset = '重置';
var qos_policy_delete_select_btn = '删除所选项';
var qos_policy_delete_all_btn = '删除所有';
var qos_policy_invalid_num = '无效的数字!';
var qos_policy_invalid_weight = '权重必须为整数!';
var qos_policy_bandwidth_empty = '在Shaping策略中，整形带宽不能为空!';
var qos_policy_bandwidth_invalid = '输入无效! 整形带宽必须为十进制数(0-9)';
var qos_policy_bandwidth_exceed_uplink = '输入无效! 整形带宽不能超过WAN上行速度.';
var qos_policy_bandwidth_exceed_downlink = '输入无效! 整形带宽不能超过WAN下行速度.';
var qos_policy_wan_empty = 'wan接口不能为空!';
var qos_policy_uplink_empty = '上行队列不能为空!';
var qos_policy_downlink_empty = '下行队列不能为空!';
var qos_policy_disable_queue = '除非必须,否则不要禁用默认队列!!';

var qos_policy_tbl_Queue = '队列';
var qos_policy_tbl_Status = '状态';
var qos_policy_tbl_Direction = '方向';
var qos_policy_tbl_Wan_Interface = 'Wan接口';
var qos_policy_tbl_Policy = '策略';
var qos_policy_tbl_Priority = '优先级';
var qos_policy_tbl_Weight = '权重';
var qos_policy_tbl_mode = '模式';
var qos_policy_tbl_bandwidth = '带宽';
var qos_policy_tbl_select = '选择';

var qos_policy_type_conflict = '队列类型只能为 PRIO, WRR 或者 SHAPING!';
var qos_policy_entry_exist = 'qos队列规则已经存在!';
var qos_policy_guarantee_uplink = '错误: 为了保证上行链路最小带宽, 所有qos规则带宽和应该比总上行链路带宽小!';
var qos_policy_guarantee_downlink = '错误: 为了保证下行链路最小带宽, 所有qos规则带宽和应该比总下行链路带宽小!';



/***********	qos_classification.htm	************/
var qos_classification_header = 'QoS分类';
var qos_classification_header_explain = ' 当前网页用于配置qos分类规则。';
var qos_classification_rule_set = 'QoS分类规则设置:';
var qos_classification_transmission = '传输:';
var qos_classification_ipver = 'Ip版本:';
var qos_classification_type = 'QoS类型:';
var qos_classification_proto = '协议:';
var qos_classification_src_ip = '源IP地址:';
var qos_classification_dst_ip = '目的IP地址:';
var qos_classification_src_port = '源端口:';
var qos_classification_dst_port = '目的端口:';
var qos_classification_src_mac = '源MAC地址:';
var qos_classification_dst_mac = '目的MAC地址:';
var qos_classification_phyport = '物理端口:';
var qos_classification_flow_label = '流程标签:';
var qos_classification_L3_precedence = 'L3优先级:';
var qos_classification_remark_set = 'QoS备注设置:';
var qos_classification_remark_802 = '备注802.1p:';
var qos_classification_table = '当前QoS分类规则表:';

var qos_classification_invalid_src_ipv4 = '无效的源IPv4地址';
var qos_classification_invalid_dst_ipv4 = '无效的目的IPv4地址';
var qos_classification_invalid_dscp = 'ipv4 dscp无效!';
var qos_classification_invalid_tos = 'ipv4 tos无效!';
var qos_classification_invalid_src_ipv4_subnet = '无效的源IPv4地址! 应该被设置在当前子网下.';
var qos_classification_invalid_dst_ipv4_subnet = '无效的目的IPv4地址! 应该被设置在当前子网下.';
var qos_classification_invalid_src_ipv6 = '无效的源IPv6地址!';
var qos_classification_invalid_dst_ipv6 = '无效的目的IPv6地址!';
var qos_classification_invalid_dscp = 'ipv6 dscp无效!';
var qos_classification_invalid_ipv6_traffic = 'ipv6事件分类无效!';
var qos_classification_field_set = '该区域也应该被设置！';
var qos_classification_invalid_src_port_range = '无效的源端口范围!\n结束端口应该比开始端口大.';
var qos_classification_invalid_dst_port_range = '无效的目的端口范围!\n结束端口应该比开始端口大.';
var qos_classification_invalid_src_port = '无效的源端口!';
var qos_classification_invalid_dst_port = '无效的目的端口!';
var qos_classification_specify_proto = '当配置端口的时候必须设置协议.';
var qos_classification_src_mac_not_complete = '输入源MAC地址不完整. 必须为12个16进制数.';
var qos_classification_invalid_src_mac = '源mac地址无效!';
var qos_classification_dst_mac_not_complete = '输入目的MAC地址不完整. 必须为12个16进制数.';
var qos_classification_invalid_dst_mac = '目的mac地址无效!';

var qos_classification_invalid_uplink_phyport = '上行链路物理端口无效!';
var qos_classification_invalid_downlink_phyport = '下行链路物理端口无效!';
var qos_classification_invalid_vlan_priority = 'vlan优先级无效!';
var qos_classification_invalid_flow_label = 'flow标签无效!';
var qos_classification_invalid_dscp_remark = 'dscp备注无效!';
var qos_classification_queue_empty = '队列不能为空!';
var qos_classification_specify_ipversion = '必须指定ip版本!';
var qos_classification_set_classification_rules = '在指定的ip版本下必须设置分类规则!';

var qos_classification_tbl_Queue = '队列';
var qos_classification_tbl_Status = '状态';
var qos_classification_tbl_Version = '版本';
var qos_classification_tbl_Protocol = '协议';
var qos_classification_tbl_src_ip = '源IP';
var qos_classification_tbl_src_port = '源端口';
var qos_classification_tbl_dst_ip = '目的IP';
var qos_classification_tbl_dst_port = '目的端口';
var qos_classification_tbl_src_mac = '源Mac';
var qos_classification_tbl_dst_mac = '目的Mac';
var qos_classification_tbl_phy_port = '物理端口';
var qos_classification_tbl_flow_label = 'Flow标签';
var qos_classification_tbl_traffic_class = '事件分类';
var qos_classification_tbl_remark_dscp = '备注DSCP';
var qos_classification_tbl_remark_802 = '备注802.1P';
var qos_classification_tbl_select = '选择';

var qos_classification_set_remark_error = '不能在没设置分类规则时设置掩码';
var qos_classification_config_dscp_tos_error = '不能再同一个队列中配置dscp和tos!';
var qos_classification_config_dscp_traffic_error = '不能再同一个队列中配置dscp和事件分类!';
var qos_classification_port_conflict = '源端口或源端口范围冲突!';
var qos_classification_dport_conflict = '目的端口或目的端口范围冲突!';
var qos_classification_entry_exist = 'qos分类规则已经存在!';


/***********	wlbasic.htm	************/
var wlbasic_header_for_double="无线基本配置-wlan";
var wlbasic_tx_restrict="发送速率限制";
var wlbasic_rx_restrict="接收速率限制";
var wlbasic_multiple_ap="虚拟AP";
var wlbasic_show='显示';

var wlbasic_header='无线基本参数设置';
var wlbasic_explain = '本页面用于配置AP的相关参数.';
var wlbasic_network_type = '网络类型:';
var wlbasic_ssid = 'SSID:';
var wlbasic_disabled = '禁用无线网络接口';
var wlbasic_country = '国家:';
var wlbasic_band= '频段:';
var wlbasic_infrastructure = "基础模式";
var wlbasic_adhoc = "自组网模式";
var wlbasic_addprofile = '增加至配置';
var wlbasic_channelwidth = '信道带宽:';
var wlbasic_ctlsideband = '控制边带:';
var wlbasic_ctlsideautomode = '自动';
var wlbasic_ctlsidelowermode = '低';
var wlbasic_ctlsideuppermode = '高';
var wlbasic_chnnelnum = '信道编号:';
var wlbasic_channelnum_auto='自动';
var wlbasic_broadcastssid= '广播SSID';
var wlbasic_brossid_enabled = '启用';
var wlbasic_brossid_disabled = '禁用';
var wlbasic_wmm ='WMM:';
var wlbasic_wmm_disabled = 	'禁用';
var wlbasic_wmm_enabled = 	'启用';
var wlbasic_data_rate = '速率:';
var wlbasic_ssid_priority = 'SSID优先级'
var wlbasic_data_rate_auto = "自动";
var wlbasic_associated_clients = '已连接的客户端:';
var wlbasic_show_associated_clients = '显示活跃的客户端';
var wlbasic_enable_mac_clone = '启用MAC地址克隆 (单一以太网客户端)';
var wlbasic_enable_repeater_mode = '启用通用中继模式(同时作为接入点和客户端)';
var wlbasic_extended_ssid = '扩展SSID:';
var wlbasic_ssid_note = '注意:如果需要更改模式和SSID设置，请务必首先在快速配置页面禁用快速配置';
var wlbasic_enable_wl_profile = '启用无线配置';
var wlbasic_wl_profile_list = '无线配置清单:';
var wlbasic_apply = '设置';
var wlbasic_reset = '重置';

var wlbasic_save = '保存';
var wlbasic_save_apply = '保存&生效';


var wlbasic_delete_select = '删除所选';
var wlbasic_delete_all = '删除全部';
var wlbasic_enable_wire = '确定增加至无线配置文件?';
var wlbasic_asloenable_wire = '确定增加至无线配置文件?';
var wlbasic_no_restrict='(0:无限制)';
var wlbasic_mode = '模式:';
var wlbasic_ap='AP';
var wlbasic_client = '客户端';
var wlbasic_wds='WDS';
var wlbasic_ap_wds='AP+WDS';
var wlbasic_ap_mesh='AP+MESH';
var wlbasic_mesh='MESH';
var wlbasic_wifi_config_ssid='SSID';
var wlbasic_wifi_config_encrypt='加密方式';
var wlbasic_wifi_config_select='选择';
var wlbasic_channelnum_auto_for_5g='自动（DFS）';
var wlbasic_set_disable_flag="设置禁用标志错误!"
var wlbasic_set_wlan_reg="Set wlan regdomain error!"
var wlbasic_set_wlan_country="Set wlan countrystr error!"
var wlbasic_set_mib_wlan_tx="Set MIB_WLAN_TX_RESTRICT error!"
var wlbasic_set_mib_wlan_rx="Set MIB_WLAN_RX_RESTRICT error!"
var wlbasic_invalid_mode="无效的模式值!"
var wlbasic_cannot_set_client="当802.1x开启时模式不能设置为客户端!<br><br>请先在安全配置页面改变加密方式."
var wlbasic_set_macclone="设置无线Mac克隆错误!"
var wlbasic_set_mib_wlan_mode_error="Set MIB_WLAN_MODE error!"
var wlbasic_set_ssid_error="设置SSID错误!"
var wlbasic_invalid_channel="无效的信道值!"
var wlbasic_set_channel_number="设置信道编号错误!"
var wlbasic_invalid_network="无效的网络类型值!"
var wlbasic_set_mib_wlan_network="Set MIB_WLAN_NETWORK_TYPE failed!"
var wlbasic_invalid_band="无效的频段值!"
var wlbasic_set_band_error="设置频段错误!"
var wlbasic_set_rate_adaptive="Set rate adaptive failed!"
var wlbasic_set_fixed_rate="Set fix rate failed!"
var wlbasic_set_tx_basic="Set Tx basic rate failed!"
var wlbasic_set_tx_operation="Set Tx operation rate failed!"
var wlbasic_invalid_channel_bond="Error! Invalid Channel Bonding."
var wlbasic_set_hidden_ssid="设置广播SSID错误!"
var wlbasic_invalid_wmm="错误! 无效的WMM值."
var wlbasic_set_mib_wlan_wmm="Set MIB_WLAN_WMM_ENABLED failed!"
var wlbasic_set_mib_wlan_channel="Set MIB_WLAN_CHANNEL_BONDING failed!"
var wlbasic_invalid_control_sideband="错误! 无效的控制边带."
var wlbasic_set_mib_wlan_control_sideband="Set MIB_WLAN_CONTROL_SIDEBAND failed!"
var wlbasic_set_tx_basic_rate="Set Tx basic rate failed!"
var wlbasic_set_tx_operation="Set Tx operation rate failed!"
var wlbasic_ssid_of_extend_length="输入的 '扩展SSID' 长度是"
var wlbasic_oversize="超出长度!最大值应该为"
var wlbasic_set_ssid_toolong="错误！SSID太长."
var wlbasic_but_your_length="但是您的SSID长度是"

/***********	wlstatbl.htm	************/
var wlstatbl_tbl_for_double='活跃的无线客户端列表-wlan';
var wlstatbl_tbl_name = '活跃的无线客户端列表';
var wlstatbl_explain = '列表显示每个已连接无线客户端的MAC地址、传输与接收分组计数值、加密状态。';
var wlstatbl_mac = 'MAC 地址';
var wlstatbl_mode = '模式';
var wlstatbl_tx = '传输分组数';
var wlstatbl_rx = '接收分组数';
var wlstatbl_tx_rate ='传输速率 (Mbps)';
var wlstatbl_ps = '节能';
var wlstatbl_expired_time = '过期时间(s)';
var wlstatbl_refresh = '刷新';
var wlstatbl_close = '关闭';
var wlstatbl_none='无';
var wlstatbl_yes='是'
var wlstatbl_no='否'

/***********	wladvanced.htm	************/
var wladv_name_for_double='无线高级设置-wlan';
var wladv_8021x_hs2='设定安全类型为802.1x之前请开启HS2守护进程';


var wladv_vallid_num_alert = '无效数字，必须是0-9的十进制数';
var wladv_fragment_thre_alert = '无效分段阈值，输入值必须是256-2346的十进制数';
var wladv_rts_thre_alert = '无效RTS阈值，输入值必须是0-2347的十进制数';
var wladv_beacon_alert = '无效信标间隔，输入值必须是20-1024的十进制数';
var wladv_header = '无线高级设置';
var wladv_explain = ' 高级设置适用于对无线局域网有充分认识的高级用户。';
var wladv_frg_thre = '分片阈值:';
var wladv_rts_thre = 'RTS 阈值:';
var wladv_beacon_interval = '信标间隔:';
var wladv_preamble_type = '前导码:';
var wladv_preamble_long = '长前导码';
var wladv_preamble_short = '短前导码';
var wladv_iapp = 'IAPP:';
var wladv_iapp_enabled = '启用';
var wladv_iapp_disabled = '禁用';
var wladv_protection = '保护机制:';
var wladv_protection_enabled = '启用';
var wladv_protection_disabled = '禁用';
var wladv_aggregation = '帧聚合:';
var wladv_aggregation_enabled = '启用';
var wladv_aggregation_disabled = '禁用';
var wladv_short_gi = '短防护时间间隔:';
var wladv_short_gi_enabled = '启用';
var wladv_short_gi_disabled = '禁用';
var wladv_wlan_partition = '无线局域网隔离:';
var wladv_wlan_partition_enabled = '启用';
var wladv_wlan_partition_disabled = '禁用';
var wladv_stbc = 'STBC:';
var wladv_stbc_enabled = '启用';
var wladv_stbc_disabled = '禁用';
var wladv_ldpc="LDPC:";
var wladv_ldpc_enabled = '启用';
var wladv_ldpc_disabled = '禁用';
var wladv_coexist = '20/40MHz 共存:';
var wladv_coexist_enabled = '启用';
var wladv_coexist_disabled = '禁用';
var wladv_tx_beamform = 'TX Beamforming:';
var wladv_tx_beamform_enabled = '启用';
var wladv_tx_beamform_disabled = '禁用';
var wladv_mu_mimo='MU MIMO:'
var wladv_mu_mimo_enabled='启用'
var wladv_mu_mimo_disabled='禁用'
var wladv_m2u = '组播转单播';
var wladv_m2u_enabled = '启用';
var wladv_m2u_disabled = '禁用';
var wladv_tdls_prohibited = 'TDLS:';
var wladv_tdls_prohibited_enabled = '启用';
var wladv_tdls_prohibited_disabled = '禁用';
var wladv_tdls_channel_switch = 'TDLS信道切换';
var wladv_tdls_channel_switch_enabled = '启用';
var wladv_tdls_channel_switch_disabled = '禁用';


var wladv_rf_power = '射频输出功率:';
var wladv_apply = '设置';
var wladv_reset = ' 重置 ';

var wladv_save = '保存';
var wladv_save_apply = '保存&生效';
var wladv_multicast_rate='组播速率:'
var wladv_multicast_auto='自动'
var wladv_80211k_support='802.11k:'
var wladv_crossband_support='自动选择中继频段:'
var wladv_enable_sta_control='信道优选'
var wladv_prefer5g='优先5GHz'
var wladv_prefer2g='优先2GHz'
var wladv_fast_bss_tran='快速BSS传输:'
var wladv_support_overds='通过分布式系统:'
var wladv_80211v_bss_tran='802.11v BSS传输:'
var wladv_smart_roaming='Smart Roaming Support:'
var wladv_enable='启用'
var wladv_disable='禁用'

/***********	wlsecutity.htm wlsecutity_all.htm	************/
var wlsec_header_for_double="无线安全设置-wlan";

var wlsec_validate_note = "注意:如果您已选择了[企业(身份认证服务器)]且更改了这里，\n 无线局域网接口及它的虚拟接口将使用目前身份认证服务器的设置，您想继续吗? ";
var wlsec_header = '无线安全设置';
var wlsec_explain = "本页面用于设置无线安全参数。";
var wlsec_select_ssid = '选择SSID:';
var wlsec_psk= 'PSK';
var wlsec_pre_shared = '预共享';
var wlsec_tkip = 'TKIP';
var wlsec_aes = 'AES';
var wlsec_apply = '设置';
var wlsec_reset = '重置';

var wlsec_save = '保存';
var wlsec_save_apply = '保存&生效';

var wlsec_inside_type_alert = "该内部类型未被支持.";
var wlsec_eap_alert = '该EAP类型未被支持.';
var wlsec_wapi_remote_ca_install_alert = '请确定从远程身份认证服务器获取的WAPI证书已经安装在[WAPI] ->[证书安装]页面.'; 
var wlsec_wapi_local_ca_install_alert = '请确定从本地身份认证服务器获取的WAPI证书已经安装在[WAPI] ->[证书安装]页面.'; 
var wlsec_wapi_wrong_select_alert = "wapi证书索引选择错误.";
var wlsec_wapi_key_length_alert = "wapi密钥最少8个字符且不多于32个字符";
var wlsec_wapi_key_hex_alert = "十六进制的WAPI密钥必须是64个十六进制数.";
var wlsec_wapi_invalid_key_alert = "无效密钥值.必须是十六进制数(0-9 or a-f).";
var wlsec_wep_confirm = "设置为WEP加密方式的情况下，WPS2.0将被禁用.";
var wlsec_wpa_confirm = "设置为WPA或TKIP的情况下，WPS2.0将被禁用";
var wlsec_wpa2_empty_alert = "WPA2 密钥套件不能为空.";
var wlsec_wpa_empty_alert = "WPA2 密钥套件不能为空.";
var wlsec_tkip_confirm = "设置为TKIP加密的情况下，WPS2.0将被禁用";
var wlsec_encryption =  '加密方式:';
var wlsec_disabled = '禁用';
var wlsec_wpa_mix = 'WPA-混合';
var wlsec_802_1x = '802.1x 认证:';
var wlsec_auth = '认证:';
var wlsec_auth_open_sys = '开放系统';
var wlsec_auth_shared_key = '共享密钥';
var wlsec_auth_auto = '自动';
var wlsec_key_length = '密钥长度:';
var wlsec_key_hex = '十六进制';
var wlsec_key_ascii = 'ASCII';
var wlsec_encryption_key = '加密密钥:';
var wlsec_auth_mode = '认证模式:';
var wlsec_auth_enterprise_mode = '企业 (RADIUS)';
var wlsec_auth_enterprise_ap_mode = '企业 (身份认证服务器)';
var wlsec_auth_personal_mode = '个人 (预共享密钥)';
var wlsec_wpa_suite = 'WPA 密钥套件:';
var wlsec_wpa2_suite = 'WPA2 密钥套件:';
var wlsec_wep_key_format = '密钥格式:';
var wlsec_pre_key_format = '预共享密钥格式:';
var wlsec_pre_key = '预共享密钥:';
var wlsec_passpharse = '口令';
var wlsec_key_hex64 = '十六进制 (64 字符)';
var wlsec_key_64bit = '64 比特';
var wlsec_key_128bit = '128 比特';
var wlsec_radius_server_ip = "RADIUS&nbsp;服务器&nbsp;IP&nbsp;地址:";
var wlsec_radius_server_port = 'RADIUS&nbsp;服务器&nbsp;端口:';
var wlsec_radius_server_password = 'RADIUS&nbsp;服务器&nbsp;密码:';
var wlsec_eap_type = 'EAP 类型:';
var wlsec_inside_tunnel_type = '内部隧道类型:';
var wlsec_eap_user_id = 'EAP 用户标识:';
var wlsec_radius_user = 'RADIUS 用户名称:';
var wlsec_radius_user_password = 'RADIUS 用户密码:';
var wlsec_user_key_password = '用户密钥密码 (if any):';
var wlsec_use_local_as = '使用本地身份认证服务器:';
var wlsec_as_ip = '身份认证服务器&nbsp;IP&nbsp;地址:';
var wlsec_select_wapi_ca = '选择WAPI证书:';
var wlsec_use_ca_from_as = '使用从远程认证服务器获取的证书';
var wlsec_adhoc_wep = '在N频段或AC频段下，Adhoc网络不支持WEP加密!';
var wlsecurity_turn_off_wpa2='在关闭企业级WPA2之前, 请先在高级设置界面关闭HS2后台进程.'
var wlsecurity_wpa_warning="警告! 只有在客户端模式支持WPA加密. <BR> 改变为WPA2加密."

/***********	tcpipwan.htm  tcpiplan.htm************/
var tcpip_lan_header = '局域网接口设置';
var tcpip_reservedip_alert='无效的IP地址!\nIP地址不能是保留IP地址(127.x.x.x).';
var tcpip_check_ip_msg = '无效的IP地址';
var tcpip_check_server_ip_msg = '无效的服务器IP地址';
var tcpip_check_dns_ip_msg1 = '无效的DNS1地址';
var tcpip_check_dns_ip_msg2 = '无效的DNS2地址';
var tcpip_check_dns_ip_msg3 = '无效的DNS3地址';
var tcpip_check_size_msg = "无效的MTU长度，必须设置为";
var tcpip_check_dns_doamin = "域名不能为空!";
var tcpip_domain_name_invalid = "域名非法！";
var tcpip_check_user_name_msg = "用户名不能为空!";
var tcpip_check_password_msg = "密码不能为空";
var tcpip_check_invalid_time_msg = "无效空闲时间值，必须设置为";
var tcpip_pppoecontype_alert = "错误的PPPOE连接类型";
var tcpip_pptpontype_alert = "错误的PPTP连接类型";
var tcpip_l2tpcontype_alert = "错误的L2TP连接类型";
var tcpip_pppcontype_alert = "错误的PPP连接类型";
var tcpip_browser_alert = '错误!您的浏览器必须支持CSS!';
var tcpip_wan_header = '广域网络接口设置';
var tcpip_wan_explain = '该页面可以设置因特网的网络参数，因特网是通过接入点的WAN接口连接的。通过改变WAN接入类型可以改变为静态IP、DHCP、PPPOE、PPTP或者L2TP等不同的接入方式。'; 

var tcpip_wan_auto_dns = '自动获取DNS';
var tcpip_wan_manually_dns =  '手动设置DNS';
var tcpip_wan_auto_aftr = '自动获取AFTR';
var tcpip_wan_manually_aftr = '手动设置AFTR';
var tcpip_wan_aftr_ipv6 = 'AFTR IPv6地址:';
var tcpip_wan_ipv6 = 'WAN IPv6地址:';
var tcpip_wan_mppe_encry = '需要MPPE加密';
var tcpip_wan_mppc_compre = '需要MPPC压缩';

var tcpip_wan_conn_time = '&nbsp;(1-1000 分钟)';
var tcpip_wan_max_mtu_size = '字节';
var tcpip_wan_conn = '连接';
var tcpip_wan_disconn = '断开连接';
var tcpip_wan_continuous = '连续';
var tcpip_wan_on_demand = '需要时连接';
var tcpip_wan_manual = '手动';
var tcpip_wan_access_type = 'WAN 接入类型:';
var tcpip_wan_type_static_ip = '静态 IP';
var tcpip_wan_type_client = "DHCP 客户端";

var tcpip_wan_type_henan = "PPPoE+(河南宽带我世界)";
var tcpip_wan_type_nanchang = "动态PPPoE(江西南昌星空极速)";
var tcpip_wan_type_other1 = "PPPoE_other1(湖南、湖北等地区星空极速)";
var tcpip_wan_type_other2 = "PPPoE_other2(湖南、湖北等地区星空极速)";
var tcpip_wan_type_dhcp_plus = "DHCP+(河南地区)";
var tcpip_wan_ip = "IP 地址:";
var tcpip_wan_mask = '子网掩码:';
var tcpip_wan_default_gateway = '默认网关:';
var tcpip_wan_mtu_size = 'MTU 长度:';
var tcpip_wan_host = '主机名称:';
var tcpip_wan_user = '用户名称:';
var tcpip_wan_pppoe_info = '第一条pppoe信息:';
var tcpip_wan_dial_num = '拨号号码:';
var tcpip_wan_not_start = '未开始';

var tcpip_wan_user = '用户名:';
var tcpip_wan_password = '密码:';
var tcpip_wan_server_ac = '服务器名称(AC):';
var tcpip_wan_conn_type = '连接时间:';
var tcpip_wan_idle_time = '空闲时间:';
var tcpip_wan_server_ip = '服务器IP地址:';
var tcpip_wan_clone_mac = '克隆MAC地址:';
var tcpip_lan_clone_mac_error = '错误! 无效的MAC地址.';
var tcpip_lan_host_error = '错误! 描述过长. ';
var tcpip_wan_clone_mac_error = '错误! 无效的MAC地址.';
var tcpip_entry_table_full ='不能添加新条目，因为表格已满!';
var tcpip_entry_ip_error = '不能添加新的条目，因为这个IP跟LAN不在一个同一个子网!';
var tcpip_ip_setted = '这个IP地址已经被设置了!';
var tcpip_mac_setted = '这个MAC地址已经被设置了!';
var tcpip_port_setted = '这个端口号已经被设置了!';
var tcpip_ip_error = '无效的IP地址! 它必须在LAN的DHCP服务器指定的IP范围内，并且不能和路由器的IP相同!';
var tcpip_ip_mac_error ='IP和MAC地址不能为空!';

var tcpip_wan_clonemac = '克隆 MAC';
var tcpip_wan_restore_defmac = '恢复默认MAC';

var tcpip_wan_enable_upnp = '&nbsp;&nbsp;启用 uPNP';
var tcpip_wan_enable_igmp_proxy = '&nbsp;&nbsp;启用IGMP代理';
var tcpip_wan_enable_ping = '&nbsp;&nbsp;在WAN上启用Ping';
var tcpip_wan_enable_webserver = '&nbsp;&nbsp;在WAN上启用Web服务器';
var tcpip_wan_web_port = 'Web接入端口:';

var tcpip_wan_enable_ipsec = '&nbsp;&nbsp;VPN连接使能IPsec通过';
var tcpip_wan_enable_pptp = '&nbsp;&nbsp;VPN连接使能PPTP通过';
var tcpip_wan_enable_l2tp = '&nbsp;&nbsp;VPN连接使能L2TP通过';
var tcpip_wan_enable_ipv6 = '&nbsp;&nbsp;VPN连接使能IPV6通过';
var tcpip_wan_enable_8021x ='&nbsp;&nbsp;在WAN上启用以太网802.1x';
var tcpip_wan_enable_netsniper = '&nbsp;&nbsp;启用防网络尖兵';
var tcpip_wan_eap_type = '&nbsp;&nbsp;&nbsp;EAP类型:';
var tcpip_wan_inside_tunnel_type = '&nbsp;&nbsp;&nbsp;内部隧道类型:';
var tcpip_wan_eap_ttls_type ='&nbsp;&nbsp;&nbsp;EAP TTLS 阶段2类型:';

var tcpip_wan_eap_userid = 'EAP用户ID:';
var tcpip_wan_radius_name = 'RADIUS用户名:';
var tcpip_wan_radius_passwd = 'RADIUS用户密码';
var tcpip_wan_user_keypasswd = '用户密钥密码(如果存在):';


var tcpip_wan_dynamic_ip = '动态IP (DHCP)';
var tcpip_wan_static_ip = '静态IP';
var tcpip_wan_attain_by_DN = '通过域名访问服务器';
var tcpip_wan_attain_by_Ip = '通过IP访问服务器';
var tcpip_wan_port_empty ='端口范围不能为空! 必须将端口的值设置为1-65535.';
var tcpip_wan_port_invalid = '无效的端口号! 必须为0-9的十进制数.';
var tcpip_wan_port_invalid_num = '无效的端口号! 必须设置端口号在1-65535之中.';
var tcpip_wan_pptp_alert1 = 'PPTP用户名不能为空!';
var tcpip_wan_pptp_alert2 = 'PPTP密码不能为空!';
var tcpip_wan_l2tp_alert1 ='L2TP用户名不能为空!';
var tcpip_wan_l2tp_alert2 ='L2TP密码不能为空!';
var tcpip_wan_ppp_alert1 = 'PPP用户名不能为空!';
var tcpip_wan_ppp_alert2 = 'PPP密码不能为空!';
var tcpip_wan_gwip_alert = '无效的pptp默认网关IP地址！\n必须是在子网内的地址。';
var tcpip_wan_inside_nosupport = '内部类型不支持.';
var tcpip_wan_phase2_nosupport = '此阶段2类型不支持.';
var tcpip_wan_eap_nosupport = 'EAP类型不支持.';
var tcpip_wan_con_su = '成功连接服务器.\n';
var tcpip_wan_con_fail = '服务器连接失败!\n';
var tcpip_wan_ppp_discon ='PPPoE断开连接.\n';
var tcpip_wan_pptp_discon = 'PPTP断开连接.\n';
var tcpip_wan_l2tp_discon = 'L2TP断开连接.\n';
var tcpip_wan_usb3g_discon = 'USB3G断开连接.\n';
var tcpip_wan_unkonwn = '未知\n';
var tcpip_wan_invalid_ip = '无效的IP地址值!';
var tcpip_wan_invalid_subnet = '无效的子网掩码值!';
var tcpip_wan_invalid_gw = '无效的默认网关值!';
var tcpip_wan_invalid_host = '无效的服务器域名！正确的域名由字母或数字或-组成且不含有空格.';
var tcpip_wan_igmpproxy = 'Nat不支持, 广播协议不能使用';
var tcpip_wan_invalid_port = '错误! 无效的端口值.';
var tcpip_wan_set_fail = '设置WAN接口条目值失败!';
var tcpip_lan_invalid_dhcp_value = '无效的DHCP值!';
var tcpip_lan_invalid_ip_value = '无效的IP地址值!';
var tcpip_lan_invalid_subnet_value = '无效的子网掩码值!';
var tcpip_lan_invalid_defgw_value ='无效的默认网关值!';
var tcpip_lan_invalid_dhcpstart = '无效的DHCP客户端起始地址!';
var tcpip_lan_invalid_dhcpend = '无效的DHCP客户端结束地址!';
var tcpip_lan_invalid_dhcprange ='无效的DHCP客户端范围!';
var tcpip_lan_invalid_dns_value = '无效的DNS地址值!';
var tcpip_lan_invalid_domain = '非法的服务器域名！正确的域名由字母或数字组成且不含有空格。';
var tcpip_check_dhcp = '请检查你的"静态DHCP"的设置. 静态IP地址必须在dhcpd的IP地址池内，并且不能和路由器地址相同!';
var tcpip_dhcp_error = 'DHCP客户端的地址不能覆盖静态DHCP表!!!';
var tcpip_change ='<h4>成功改变设置!<BR><BR>在此期间内不要关掉或重启设备。</h4>';
var tcpip_reboot = '重启检查失败!\n';
var tcpip_wan_invalid_dns_mode = '无效的DNS模式值!';
var tcpip_wan_invalid_ppp_type = '无效的PPP类型值!';
var tcpip_wan_invalid_pptp_type ='无效的PPTP的广域网IP类型!';
var tcpip_wan_invalid_defgw = '无效的pptp默认网关值!';
var tcpip_wan_invalid_pptp_server = '无效的pptp服务器ip值!';
var tcpip_wan_invalid_pptp_type ='无效的PPTP类型值!';
var tcpip_wan_invalid_l2tp_type = '无效的L2TP的广域网IP类型!';
var tcpip_wan_invalid_l2tp_defgw ='无效的l2tp默认网关值!';
var tcpip_wan_invalid_l2tp_server = '无效的l2tp服务器ip值!';
var tcpip_wan_invalid_l2tp = '无效的L2TP类型值!';
var tcpip_wan_invalid_usb ='无效的USB3G类型值!';
var tcpip_wan_invalid_ds = '无效的ds-lite类型值!';
var tcpip_wan_invalid_aftr ='无效的AFTR地址值!';
var tcpip_wan_invalid_ip_mode = '无效的IP模型值!';
var tcpip_wan_invalid_8021x = '无效的802.1x EAP类型值!';
var tcpip_wan_8021x = '没有802.1x EAP类型!';
var tcpip_wan_8021x_user = '没有802.1x EAP用户ID!';
var tcpip_wan_8021x_user2 = '没有802.1x RADIUS用户名!';
var tcpip_wan_8021x_user3 = '没有802.1x RADIUS用户密码!';
var tcpip_wan_8021x_tunnel ='无效的802.1x内部通道类型值!';
var tcpip_wan_8021x_tunnel2 = '没有802.1x内部通道类型!';
var tcpip_wan_8021x_tunnel3 = '不支持802.1x内部通道类型!';
var tcpip_wan_8021x_phase ='无效的802.1x阶段2类型值!';
var tcpip_wan_8021x_phase2 ='没有802.1x阶段2类型!';
var tcpip_wan_eap_long = 'EAP用户ID太长!';
var tcpip_wan_ttls_phase2 = '802.1x ttls的阶段2类型不支持!';
var tcpip_wan_eap_nsupport = '802.1x EAP类型不支持!';
var tcpip_wan_upnp = 'Upnp只能运行在一个WAN接口上\n';

var tcpip_wan_con_su = '成功连接服务器.\n';
var tcpip_wan_con_fail = '服务器连接失败!\n';
var tcpip_wan_ppp_discon ='PPPoE断开连接.\n';
var tcpip_wan_pptp_discon = 'PPTP断开连接.\n';
var tcpip_wan_l2tp_discon = 'L2TP断开连接.\n';
var tcpip_wan_usb3g_discon = 'USB3G断开连接.\n';
var tcpip_wan_unkonwn = '未知\n';
var tcpip_wan_invalid_ip = '无效的IP地址值!';
var tcpip_wan_invalid_subnet = '无效的子网掩码值!';
var tcpip_wan_invalid_gw = '无效的默认网关值!';
var tcpip_wan_invalid_host = '无效的服务器域名！正确的域名由字母或数字或-组成且不含有空格.';
var tcpip_wan_invalid_port = '错误! 无效的端口值.';
var tcpip_wan_set_fail = '设置WAN接口条目值失败!';

var tcpip_lan_invalid_dhcp_value = '无效的DHCP值!';
var tcpip_lan_invalid_ip_value = '无效的IP地址值!';
var tcpip_lan_invalid_subnet_value = '无效的子网掩码值!';
var tcpip_lan_invalid_defgw_value ='无效的默认网关值!';
var tcpip_lan_invalid_dhcpstart = '无效的DHCP客户端起始地址!';
var tcpip_lan_invalid_dhcpend = '无效的DHCP客户端结束地址!';
var tcpip_lan_invalid_dhcprange ='无效的DHCP客户端范围!';
var tcpip_lan_invalid_dns_value = '无效的DNS地址值!';
var tcpip_lan_invalid_domain = '非法的服务器域名！正确的域名由字母或数字组成且不含有空格。';
var tcpip_check_dhcp = '请检查你的"静态DHCP"的设置. 静态IP地址必须在dhcpd的IP地址池内，并且不能和路由器地址相同!';
var tcpip_dhcp_error = 'DHCP客户端的地址不能覆盖静态DHCP表!!!';
var tcpip_change ='<h4>成功改变设置!<BR><BR>在此期间内不要关掉或重启设备。</h4>';
var tcpip_reboot = '重启检查失败!\n';
var tcpip_wan_invalid_dns_mode = '无效的DNS模式值!';
var tcpip_wan_invalid_ppp_type = '无效的PPP类型值!';
var tcpip_wan_invalid_pptp_type ='无效的PPTP的广域网IP类型!';
var tcpip_wan_invalid_defgw = '无效的pptp默认网关值!';
var tcpip_wan_invalid_pptp_server = '无效的pptp服务器ip值!';
var tcpip_wan_invalid_pptp_type ='无效的PPTP类型值!';
var tcpip_wan_invalid_l2tp_type = '无效的L2TP的广域网IP类型!';
var tcpip_wan_invalid_l2tp_defgw ='无效的l2tp默认网关值!';
var tcpip_wan_invalid_l2tp_server = '无效的l2tp服务器ip值!';
var tcpip_wan_invalid_l2tp = '无效的L2TP类型值!';
var tcpip_wan_invalid_usb ='无效的USB3G类型值!';
var tcpip_wan_invalid_ds = '无效的ds-lite类型值!';
var tcpip_wan_invalid_aftr ='无效的AFTR地址值!';
var tcpip_wan_invalid_ip_mode = '无效的IP模型值!';
var tcpip_wan_invalid_8021x = '无效的802.1x EAP类型值!';
var tcpip_wan_8021x = '没有802.1x EAP类型!';
var tcpip_wan_8021x_user = '没有802.1x EAP用户ID!';
var tcpip_wan_8021x_user2 = '没有802.1x RADIUS用户名!';
var tcpip_wan_8021x_user3 = '没有802.1x RADIUS用户密码!';
var tcpip_wan_8021x_tunnel ='无效的802.1x内部通道类型值!';
var tcpip_wan_8021x_tunnel2 = '没有802.1x内部通道类型!';
var tcpip_wan_8021x_tunnel3 = '不支持802.1x内部通道类型!';
var tcpip_wan_8021x_phase ='无效的802.1x阶段2类型值!';
var tcpip_wan_8021x_phase2 ='没有802.1x阶段2类型!';
var tcpip_wan_eap_long = 'EAP用户ID太长!';
var tcpip_wan_ttls_phase2 = '802.1x ttls的阶段2类型不支持!';
var tcpip_wan_eap_nsupport = '802.1x EAP类型不支持!';
var tcpip_wan_upnp = 'Upnp只能运行在一个WAN接口上\n';




var tcpip_lan_save = '保存';
var tcpip_wan_save = '保存';
var tcpip_save_apply = '保存&生效';
var tcpip_reset = '重置';
var tcpip_apply ='设置';

var tcpip_lan_wrong_dhcp_field = "错误的dhcp域!";
var tcpip_lan_start_ip = '无效的DHCP客户端起始地址';
var tcpip_lan_end_ip = '无效的DHCP客户端结束地址';
var tcpip_lan_ip_alert = '\n必须是在子网内的地址。';
var tcpip_lan_invalid_rang = '无效的客户端地址范围\n结束地址必须大于起始地址。';
var tcpip_lan_invalid_gw_ip = '网关地址无效！'; 
var tcpip_lan_macaddr_nocomplete = '输入的MAC地址不完整，MAC地址必须是12个16进制的长度.';
var tcpip_lan_macaddr_invlaid_value1 ='无效的MAC地址. MAC地址不能是ff:ff:ff:ff:ff:ff.';
var tcpip_lan_macaddr_invlaid_value2 ='无效的MAC地址. MAC地址不能是多播MAC地址.';
var tcpip_lan_macaddr_invlaid_value3 = '无效的MAC地址. MAC地址不能是00:00:00:00:00:00.';
var tcpip_lan_macaddr_invlaid_value4 = '无效的MAC地址. MAC地址不能是位于01:00:5e:00:00:00到01:00:5e:7f:ff:ff的多播地址.';

var tcpip_lan_macaddr_invalid = '无效的MAC地址,MAC地址必须是16进制.';
var tcpip_lan_invalid_dhcp_lease_time = 'DHCP租约时间无效!';
var tcpip_lan_invalid_domain_name = '无效的域名名称! 域名名称长度不能超过63个字符';
var tcpip_lan_invalid_rang_value = "无效值. 必须在 (1 ~ 10080)范围内.";
var tcpip_lan_invalid_dhcp_type = "载入错误的dhcp类型!";
var tcpip_lan_explain = '该页面可以配置连接到接入点LAN端口的局域网网络参数，配置IP地址，子网掩码，DHCP等参数';
var tcpip_lan_ip = "IP地址:";
var tcpip_lan_mask = '子网掩码:';
var tcpip_lan_default_gateway = '默认网关:';
var tcpip_lan_dhcp = 'DHCP:';
var tcpip_lan_dhcp_disabled = '禁用';
var tcpip_lan_dhcp_client = '客户端';
var tcpip_lan_dhcp_server = '服务器';
var tcpip_lan_dhcp_auto = '自动';
var tcpip_lan_dhcp_rang = 'DHCP 客户端范围:';
var tcpip_lan_dhcp_time = 'DHCP 租约时间:';
var tcpip_minutes = '分钟';
var tcpip_lan_staicdhcp = '静态 DHCP:';
var tcpip_staticdhcp = "设置 DHCP";
var tcpip_domain = "域名名称:";
var tcpip_netbios = "NetBIOS 名称:";
var tcpip_802_1d = "802.1d 生成树:";
var tcpip_802_1d_enable = '启用';
var tcpip_802_1d_disabled = '禁用';
var tcpip_show_client = '显示客户端';
var tcpip_lan_invalid_lan_ipv6 = '局域网地址无效!';
var tcpip_lan_invalid_ula_ipv6 = 'ULA地址无效!';
var tcpip_lan_invalid_dns = 'DNS地址无效!';
var tcpip_lan_invalid_start_ip = '起始地址无效!';
var tcpip_lan_invalid_end_ip = '结束地址无效!';
var tcpip_lan_invalid_first_prefix = '第一个前缀地址无效!';
var tcpip_lan_invalid_second_prefix = '第二个前缀地址无效!';
var tcpip_lan_wac_if = 'WAC接口:';

var tcpip_lan_ipv6_config = '配置IPv6局域网设置'; 
var tcpip_lan_ipv6_auto_config = '自动配置Ipv6局域网';
var tcpip_lan_ipv6_manu_config = '手动配置Ipv6局域网';
var tcpip_lan_prefix = '前缀长度';
var tcpip_lan_ula_config = '<b>配置ULA设置</b> <BR>  <BR>';
var tcpip_lan_enable_ula = '开启ULA';
var tcpip_lan_manu_ula = '手动设置ULA';
var tcpip_lan_auto_ula = '自动设置ULA';
var tcpip_lan_dhcpv6_config = '配置DHCPv6服务器'; 
var tcpip_lan_enable = '开启:';
var tcpip_lan_dns = 'DNS地址:';
var tcpip_lan_if = '接口名称:';
var tcpip_lan_pool = '地址池:';
var tcpip_lan_addr_prefix = '地址前缀:';
var tcpip_lan_manu = '手动';
var tcpip_lan_pd = '地址委派';
var tcpip_lan_ula_prefix = 'ULA前缀';
var tcpip_lan_from = '起始地址:';
var tcpip_lan_to = '结束地址:';
var tcpip_lan_router_ad = '配置路由器通告';
var tcpip_lan_router_ad_name = '路由通告名称:';
var tcpip_lan_max_router_ad_interval = '最大路由器通告间隔:';
var tcpip_lan_min_router_ad_interval = '最小路由器通告间隔:';
var tcpip_lan_min_delay = '最小路由通告延迟:';
var tcpip_lan_ad_flag = '通告管理标志:';
var tcpip_lan_ad_other_flag = '其他通告配置标志:';
var tcpip_lan_ad_mtu = '通告链路MTU:';
var tcpip_lan_ad_time = '通告可达时间:';
var tcpip_lan_ad_timer = '通告传输时间:';
var tcpip_lan_ad_cur_hop = '当前通告跳数限制:';
var tcpip_lan_ad_def_lifetime = '默认通告生存时间:';
var tcpip_lan_ad_def_prefer = '默认通告选择:';
var tcpip_lan_ad_src_lladdr = '源LL地址:';
var tcpip_lan_unicast = '仅支持单播:';
var tcpip_lan_config_prefix1 = '配置前缀1';
var tcpip_lan_prefix_mode = '前缀模式:';
var tcpip_lan_pre = '前缀:';
var tcpip_lan_ad_linkflag = '通告链路标志:';
var tcpip_lan_auto_flag = '自动通告标志:';
var tcpip_lan_valid_lifetime = '通告有效生存时间:';
var tcpip_lan_prefer_lifetime = '通告首选生存时间:';
var tcpip_lan_router_addr = '通告路由地址:';
var tcpip_lan_config_prefix2 = '配置前缀2';


var tcpip_l2tp_server_domain_name = '非法的服务器域名！正确的域名由字母或数字组成且不含有空格。';
var tcpip_l2tp_server_domain_name2='非法的服务器域名! 正确的域名由字母开头并且以字母或数字结尾。';

var tcpip_invalid_pri = '优先级值无效!';
var tcpip_vid ='虚拟局域网id';
var tcpip_vid_reserve  =' 是用于内部保留使用!';
var tcpip_vid_conflict  ='虚拟局域网id冲突于wan';




/***********	dhcptbl.htm ************/
var dhcp_header = '活跃的DHCP客户端列表';
var dhcp_explain = '该表显示了每个已租约的DHCP客户的分配的IP地址、MAC地址、租约时间期限。';
var dhcp_devname = '设备名称';
var dhcp_ip = 'IP 地址';
var dhcp_mac = 'MAC 地址';
var dhcp_time = '期限时间(s)';
var dhcp_refresh = '刷新';
var dhcp_close = '关闭';
var dhcp_none = '无';
var dhcp_always = '永久';


/***********	ipv6_wan.htm ************/
var ipv6_wan_header = '广域网络接口设置';
var ipv6_wan_header_explain = '该页面可以设置因特网的网络参数，因特网是通过接入点的WAN接口连接的。通过改变WAN接入类型可以改变为静态IP、DHCP、PPPOE、PPTP或者L2TP等不同的接入方式。';
var ipv6_wan_ppp_alert1 = 'PPP用户名不能为空!';
var ipv6_wan_ppp_alert2 = 'PPP密码不能为空!';
var ipv6_wan_alert1 = '错误! 你的浏览器必须能支持CSS!';
var ipv6_wan_invalid_static_ip ='静态IP地址无效!';
var ipv6_wan_invalid_gw = '网关地址无效!';
var ipv6_wan_pppoe_alert1 = 'pppoe用户名不能为空';
var ipv6_wan_pppoe_alert2 = 'pppoe密码不能为空';
var ipv6_wan_invalid_pppoe_mtu = '无效的pppoe mtu大小';
var ipv6_wan_invalid_dns1 = 'dns1地址无效!';
var ipv6_wan_invalid_dns2 = 'dns2地址无效!';
var ipv6_wan_invalid_mac = 'mac地址无效!';
var ipv6_wan_invalid_6rd = '6RD前缀地址无效!';
var ipv6_wan_6rd_alert1 = '6RD前缀长度必须是0到64之间的8的倍数!';
var ipv6_wan_6rd_alert2 = '6RD前缀长度必须是0到32之间的8的倍数!';
var ipv6_wan_6rd_alert2 = '6RD BR地址无效!';
var ipv6_wan_enable = '启用IPv6';
var ipv6_wan_origin_type = '源类型:';
var ipv6_wan_auto = '自动';
var ipv6_wan_static = '静态';
var ipv6_wan_link_type = 'WAN链接类型:';
var ipv6_wan_ether = '以太网';
var ipv6_wan_username = '用户名:';
var ipv6_wan_password = '密码:';
var ipv6_wan_service_name = '服务名:';
var ipv6_wan_ac_name = 'AC名:';
var ipv6_wan_con_type = '连接类型:';
var ipv6_wan_continuous = '连续';
var ipv6_wan_on_demand = '需要时连接';
var ipv6_wan_manual = '手动';
var ipv6_wan_conn= '连接';
var ipv6_wan_disconn = '断开连接';
var ipv6_wan_idle_time = '空闲时间:';
var ipv6_wan_mtu_size  = 'MTU大小:';
var ipv6_wan_minutes = '分钟';
var tcpip_wan_bytes = '字节';
var ipv6_wan_static_ip = '静态Ip';
var ipv6_wan_ip_addr = 'IP地址:';
var ipv6_wan_prefix = '前缀长度';
var ipv6_wan_def_gw = '默认网关:';
var ipv6_wan_stateless_addr = '无状态地址自动配置';
var ipv6_wan_stateful_addr = '状态地址自动配置';
var ipv6_wan_pd = 'PD开启:';
var ipv6_wan_rapid = '快速分配开启:';
var ipv6_wan_6rd_ipv6_prefix = '6RD IPv6前缀:';
var ipv6_wan_ipv4_addr = 'IPv4地址:';
var ipv6_wan_mask_len = '掩码长度';
var ipv6_wan_6rd_border_relay_ipv4 = '6RD边界中继IPv4地址:';
var ipv6_wan_dns_set = 'Dns设置';
var ipv6_wan_dnsv6 ='配置DNSv6';
var ipv6_wan_auto_dns = '自动获取DNS';
var ipv6_wan_manually_dns =  '手动设置DNS';
var ipv6_wan_other = '其他';
var ipv6_wan_dslite = '开启DSLITE';
var ipv6_wan_mld ='开启MLD代理';
var ipv6_wan_clone_mac = '克隆MAC地址:';

var ipv6_wan_save = '保存';
var ipv6_wan_save_apply = '保存&生效';
var ipv6_wan_reset = '重置';
var ipv6_wan_apply ='设置';

/***********	dhcp6s.htm ************/
var dhcp6s_header = '局域网设置';
var dhcp6s_invalid_lan_ipv6 = '局域网地址无效!';
var dhcp6s_invalid_ula_ipv6 = 'ULA地址无效!';
var dhcp6s_invalid_dns = 'DNS地址无效!';
var dhcp6s_invalid_start_ip = '起始地址无效!';
var dhcp6s_invalid_end_ip = '结束地址无效!';
var dhcp6s_ip_addr = 'IP地址:';
var dhcp6s_prefix_len = '前缀长度';
var dhcp6s_ula_config = '配置ULA设置';
var dhcp6s_enable_ula = '开启ULA';
var dhcp6s_manu_ula = '手动设置ULA';
var dhcp6s_auto_ula = '自动设置ULA';
var dhcp6s_dhcpv6_config = '配置DHCPv6服务器'; 
var dhcp6s_enable = '开启:';
var dhcp6s_dns = 'DNS地址:';
var dhcp6s_if = '接口名称:';
var dhcp6s_pool = '地址池:';
var dhcp6s_addr_prefix = '地址前缀:';
var dhcp6s_manu = '手动';
var dhcp6s_pd = '地址委派';
var dhcp6s_ula_prefix = 'ULA前缀';
var dhcp6s_from = '起始地址:';
var dhcp6s_to = '结束地址:';
var dhcp6s_save='保存';
var dhcp6s_save_apply='保存 & 设置';

/***********	radvd.htm ************/
var radvd_header = '配置路由器通告';
var radvd_invalid_first_prefix = '第一个前缀地址无效!';
var radvd_invalid_second_prefix = '第二个前缀地址无效!';
var radvd_enable = '开启';
var radvd_router_ad_name = '路由通告名称:';
var radvd_max_router_ad_interval = '最大路由器通告间隔:';
var radvd_min_router_ad_interval = '最小路由器通告间隔:';
var radvd_min_delay = '最小路由通告延迟:';
var radvd_ad_flag = '通告管理标志:';
var radvd_ad_other_flag = '其他通告配置标志:';
var radvd_ad_mtu = '通告链路MTU:';
var radvd_ad_time = '通告可达时间:';
var radvd_ad_timer = '通告传输时间:';
var radvd_ad_cur_hop = '当前通告跳数限制:';
var radvd_ad_def_lifetime = '默认通告生存时间:';
var radvd_ad_def_prefer = '默认通告选择:';
var radvd_ad_src_lladdr = '源LL地址:';
var radvd_unicast = '仅支持单播:';
var radvd_prefix1 = '前缀1';
var radvd_prefix2 = '前缀2';
var radvd_enable_mode ='启用';
var radvd_prefix_mode = '前缀模式:';
var radvd_pre = '前缀:';
var radvd_ad_linkflag = '通告链路标志:';
var radvd_auto_flag = '自动通告标志:';
var radvd_valid_lifetime = '通告有效生存时间:';
var radvd_prefer_lifetime = '通告首选生存时间:';
var radvd_router_addr = '通告路由地址:';
var radvd_manu = '手动';
var radvd_pd = '地址委派';
var radvd_ula_prefix ='ULA前缀';
var radvd_pre2 = '前缀';
var radvd_ad_linkflag = '通告链路标志:';
var radvd_auto_flag = '自动通告标志:';
var radvd_valid_lifetime = '通告有效生存时间:';
var radvd_prefer_lifetime = '通告首选生存时间:';
var radvd_router_addr = '通告路由地址:';
var radvd_ssave = '保存';
var radvd_save_apply = '保存 & 设置';
var radvd_default = '默认';
var radvd_reset = '重置';

/***********	tunnel6.htm ************/
var tunnel6_header = '配置隧道(6to4)';
var tunnel6_enable = '开启';
var tunnel6_save = '保存';


/***********	map_e.htm ************/
var map_e_header = 'MAP-E设置';
var map_e_header_explain = '该页面用来配置封装的地址和端口映射的参数。';
var map_e_alert1 = 'PSID偏移量必须在0-16之间!';
var map_e_alert2 = 'PSID长度必须在0-16之间!';
var map_e_enable = '开启';
var map_e_auto ='自动';
var map_e_manu ='手动';
var map_e_ipv6 = 'IPv6前缀';
var map_e_ipv4 = 'IPv4前缀';
var map_e_br = 'BR地址';
var map_e_psid_offset = 'PSID偏移量';
var map_e_psid_len = 'PSID长度';
var map_e_apply = '设置';





/***********	util_gw.js ************/
var util_gw_wps_warn1 = 'WPS已经配置了SSID。设置的任何改变';
var util_gw_wps_warn2 = 'WPS已经配置了接入点模式。设置的任何改变';
var util_gw_wps_warn3 = 'WPS已经配置了安全设置。设置的任何改变';
var util_gw_wps_warn4 = 'WPS不支持WPA企业认证。';
var util_gw_wps_warn5 = 'WPS不支持802.1x认证。';
var util_gw_wps_warn6 = 'WPS不支持无线分布式系统模式。';
var util_gw_wps_warn7 = 'WPS不支持自组织网络模式。 ';
var util_gw_wps_cause_disconn = '会导致工作站断开连接。';
var util_gw_wps_want_to = '确定继续更改新的设置?';
var util_gw_wps_cause_disabled = '使用该配置将会导致禁用。 ';
var util_gw_wps_ecrypt_11n = '无效的加密模式! WPA or WPA2, AES密码套件 必须使用802.11n的频段。';
var util_gw_wps_ecrypt_basic = '加密模式不适用于802.11n的频段。 请更改无线局域网络的加密设置，否则工作将会不正确。';
var util_gw_wps_ecrypt_confirm = '确定继续把该加密模式应用在802.11n的频段上? 用户使用无线局域网络不会得到良好性能!';
var util_gw_ssid_hidden_alert = "如果开启隐藏SSID; WPS2.0将会被禁用。";
var util_gw_ssid_empty = 'SSID不能为空。';
var util_gw_preshared_key_length_alert =  '预共享密钥必须是64个字符。';
var util_gw_preshared_key_alert = "预共享密钥值无效。必须是十六进制数(0-9 or a-f)。";
var util_gw_preshared_key_min_alert = '预共享密钥值设置值最少设置为8个字符。';
var util_gw_preshared_key_max_alert = '预共享密钥值设置不能超过64个字符。';
var util_gw_decimal_rang = '必须是1-65535的十进制数。';
var util_gw_invalid_radius_port = 'RADIUS服务器端口值无效! ';
var util_gw_empty_radius_port = "RADIUS服务器端口值不能为空! ";
var util_gw_invalid_radius_ip = 'RADIUS服务器IP地址无效';
var util_gw_mask_empty = '子网掩码不能为空! ';
var util_gw_ip_format = '必须填充为4个数字，如 xxx.xxx.xxx.xxx.';
var util_gw_mask_error = '子网掩码不能是0.0.0.0!';
var util_gw_mask_rang = '\n必须是这些数字: 0, 128, 192, 224, 240, 248, 252 or 254';
var util_gw_mask_rang1 = '\n必须是这些数字: 128, 192, 224, 240, 248, 252 or 254';
var util_gw_mask_invalid1 = '子网掩码的第一个数字无效。';
var util_gw_mask_invalid2 = '子网掩码的第二个数字无效。';
var util_gw_mask_invalid3 = '子网掩码的第三个数字无效。';
var util_gw_mask_invalid4 = '子网掩码的第四个数字无效。';
var util_gw_mask_invalid = '子网掩码值无效。';
var util_gw_decimal_value_rang = "必须是十进制数 (0-9).";
var util_gw_invalid_degw_ip = '默认网关地址无效';
var util_gw_invalid_gw_ip = '网关地址无效!';
var util_gw_locat_subnet = '\n必须是同一子网内的当前IP地址。';
var util_gw_mac_complete = '输入的MAC地址不完全。 ';
var util_gw_mac_empty = '输入的MAC地址为空！ ';
var util_gw_mac_zero = '输入的MAC地址不能为全0! ';
var util_gw_mac_ff = '输入的MAC地址不能为全F! ';
var util_gw_12hex = '必须是12个十六进制数。';
var util_gw_invalid_mac = 'MAC地址无效。 ';
var util_gw_ff = '它不应该是ff:ff:ff:ff:ff:ff.';
var util_gw_multicast = '它不应该是多播MAC地址';

var util_gw_hex_rang = '必须是十六进制数(0-9 或者 a-f).';
var util_gw_ip_empty = 'IP地址不能为空! ';
var util_gw_invalid_value = ' 值。 ';
var util_gw_should_be = '必须是 ';
var util_gw_check_ppp_rang1 = ' 值域的第一个数字。';
var util_gw_check_ppp_rang2 = ' 值域的第二个数字。';
var util_gw_check_ppp_rang3 = ' 值域的第三个数字。';
var util_gw_check_ppp_rang4 = ' 值域的第四个数字。';
var util_gw_check_ppp_rang5 = ' 值域的第五个数字。';
var util_gw_invalid_key_length = '密钥长度值无效 ';
var util_gw_char = ' 字符。';
var util_gw_invalid_wep_key_value = 'WEP密钥长度值无效。';
var util_gw_invalid_key_value = '密钥值无效。 ';
var util_gw_invalid_ip = '无效的IP地址';
var util_gw_check_server_ip_msg = '无效的服务器IP地址';
var util_gw_wan_gwip_alert = '无效的pptp默认网关IP地址！\n必须是在子网内的地址.';
var util_gw_wan_gwip_alert2 ='无效的l2tp默认网关IP地址！\n必须是在子网内的地址.';
var util_gw_check_dns_ip_msg1 = '无效的DNS1地址';
var util_gw_check_dns_ip_msg2 = '无效的DNS2地址';
var util_gw_check_dns_ip_msg3 = '无效的DNS3地址';

var util_gw_check_user_name_msg = "用户名不能为空!";
var util_gw_check_password_msg = "密码不能为空";


var util_gw_ipaddr_empty = 'IP地址不能为空! IP地址是xxx.xxx.xxx.xxx形式的4位数.';
var util_gw_ipaddr_nodecimal = '值,必须为0-9的十进制数.';
var util_gw_ipaddr_1strange1 =  '域在第1位,必须为1-223.';
var util_gw_ipaddr_1strange2 = '域在第1位,值不能是127.';
var util_gw_ipaddr_1strange3 = '域在第1位,必须为0-255.';
var util_gw_ipaddr_1strange = '域在第1位,必须为0-255.';
var util_gw_ipaddr_1range='域在第1位,必须为1-223.';
var util_gw_ipaddr_1range_for127=' 域在第1位,不能为127.';
var util_gw_ipaddr_2ndrange = '域在第2位,必须为0-255.';
var util_gw_ipaddr_3rdrange = '域在第3位,必须为0-255.';
var util_gw_ipaddr_4thrange = '域在第4位,必须为1-254.';
var util_gw_ipaddr_5thrange = '域在第4位,必须为1-32.';
var util_gw_tx_restrict='发送速率限制不能为空!';
var util_gw_tx_restrict_limit='发送速率限制应在0-1000之间!';
var util_gw_rx_restrict='接收速率限制不能为空!';
var util_gw_rx_restrict_limit='接收速率限制应在0-1000之间!';


var util_gw_ipv6_invalid =' ipv6地址无效!';
var util_gw_ipv6_prefix = ' ipv6前缀必须是0-128!';
var util_gw_mustbetween = ' 必须处于 ';
var util_gw_or = '或者';
var util_gw_field = '此处也应该被设置';



var util_gw_array0 = "(GMT-12:00)埃尼威托克岛, 夸贾林环礁";
var util_gw_array1 = "(GMT-11:00)中途岛, 萨摩亚";
var util_gw_array2 = "(GMT-10:00)夏威夷";
var util_gw_array3 = "(GMT-09:00)阿拉斯加州";
var util_gw_array4 = "(GMT-08:00)太平洋时间(美国和加拿大); 提华纳";
var util_gw_array5 = "(GMT-07:00)亚利桑那州";
var util_gw_array6 = "(GMT-07:00)山地时区(美国和加拿大)";
var util_gw_array7 = "(GMT-06:00)中央时间(美国&加拿大)";
var util_gw_array8 = "(GMT-06:00)墨西哥城, 特古西加尔巴";
var util_gw_array9 = "(GMT-06:00)萨斯喀彻温省";
var util_gw_array10 = "(GMT-05:00)波哥大, 利马, 基多";
var util_gw_array11 = "(GMT-05:00)东部时间(美国&加拿大)";
var util_gw_array12 = "(GMT-05:00)印第安纳州(东)";
var util_gw_array13 = "(GMT-04:00)大西洋时间(加拿大)";
var util_gw_array14 = "(GMT-04:00)加拉加斯,拉巴斯";
var util_gw_array15 = "(GMT-04:00)圣地亚哥";
var util_gw_array16 = "(GMT-03:30)纽芬兰";
var util_gw_array17 = "(GMT-03:00)巴西利亚";
var util_gw_array18 = "(GMT-03:00)布宜诺斯艾利斯,乔治城";
var util_gw_array19 = "(GMT-02:00)大西洋中部";
var util_gw_array20 = "(GMT-01:00)亚速尔群岛,佛得角.";
var util_gw_array21 = "(GMT)卡萨布兰卡,蒙罗维亚";
var util_gw_array22 = "(GMT)格林威治标准时间:都柏林爱丁堡、里斯本、伦敦";
var util_gw_array23 = "(GMT+01:00)阿姆斯特丹、柏林、伯尔尼、罗马、斯德哥尔摩、维也纳";
var util_gw_array24 = "(GMT+01:00)贝尔格莱德,布拉迪斯拉发、布达佩斯、卢布尔雅那,布拉格";
var util_gw_array25 = "(GMT+01:00)巴萨,皇马";
var util_gw_array26 = "(GMT+01:00)布鲁塞尔,哥本哈根,马德里,巴黎,维尔纽斯";
var util_gw_array27 = "(GMT+01:00)巴黎";
var util_gw_array28 = "(GMT+01:00)萨拉热窝,斯科普里,索非亚,华沙,萨格勒布";
var util_gw_array29 = "(GMT+02:00)雅典,伊斯坦布尔,明斯克";
var util_gw_array30 = "(GMT+02:00)布加勒斯特";
var util_gw_array31 = "(GMT+02:00)开罗";
var util_gw_array32 = "(GMT+02:00)哈拉雷,比勒陀利亚";
var util_gw_array33 = "(GMT+02:00)赫尔辛基,包括里加、塔林";
var util_gw_array34 = "(GMT+02:00)耶路撒冷";
var util_gw_array35 = "(GMT+03:00)巴格达,科威特,利雅得";
var util_gw_array36 = "(GMT+03:00)莫斯科、圣彼得堡、伏尔加格勒";
var util_gw_array37 = "(GMT+03:00)内罗比";
var util_gw_array38 = "(GMT+03:30)德黑兰";
var util_gw_array39 = "(GMT+04:00)阿布扎比,马斯喀特";
var util_gw_array40 = "(GMT+04:00)巴库第比利斯";
var util_gw_array41 = "(GMT+04:30)喀布尔";
var util_gw_array42 = "(GMT+05:00)叶卡捷琳堡";
var util_gw_array43 = "(GMT+05:00)伊斯兰堡,巴基斯坦卡拉奇,塔什干";
var util_gw_array44 = "(GMT+05:30)孟买、加尔各答、马德拉斯,新德里";
var util_gw_array45 = "(GMT+06:00)阿斯塔纳、阿拉木图、达卡";
var util_gw_array46 = "(GMT+06:00)科伦坡";
var util_gw_array47 = "(GMT+07:00)曼谷、河内、雅加达";
var util_gw_array48 = "(GMT+08:00)北京、重庆、香港、乌鲁木齐";
var util_gw_array49 = "(GMT+08:00)珀斯";
var util_gw_array50 = "(GMT+08:00)新加坡";
var util_gw_array51 = "(GMT+08:00)台北";
var util_gw_array52 = "(GMT+09:00)大阪,札幌,东京";
var util_gw_array53 = "(GMT+09:00)首尔";
var util_gw_array54 = "(GMT+09:00)雅库茨克";
var util_gw_array55 = "(GMT+09:30)阿德莱德";
var util_gw_array56 = "(GMT+09:30)达尔文";
var util_gw_array57 = "(GMT+10:00)布里斯班";
var util_gw_array58 = "(GMT+10:00)堪培拉、墨尔本、悉尼";
var util_gw_array59 = "(GMT+10:00)关岛,莫尔兹比港";
var util_gw_array60 = "(GMT+10:00)霍巴特";
var util_gw_array61 = "(GMT+10:00)海参崴";
var util_gw_array62 = "(GMT+11:00)马加丹州,所罗门群岛。,新喀里多尼亚";
var util_gw_array63 = "(GMT+12:00)奥克兰, 惠灵顿";
var util_gw_array64 = "(GMT+12:00)斐济, 堪察加半岛, 马绍尔群岛.";

var util_gw_chanauto = '自动';
var uyi_gw_chan_dfsauto = '自动(DFS)';
var util_gw_bcast_mcast_mac= '不能设置广播或多播MAC地址';
var util_invalid_mac_addr_multicast='无效的MAC地址，MAC地址不应为01:00:5e:00:00:00到01:00:5e:7f:ff:ff的多播MAC地址.';

var util_save_wan1 = '广域网的IP地址和局域网的IP地址不能在相同的网段.';
var util_save_wan2 = '广域网静态IP地址不能跟网关IP地址相同.';
var util_save_wan3 = '无效的DNS地址';
var util_save_mtu = 'MTU大小';
var util_save_wan4 = '无效的服务器域名! 域名的长度不能大于63.';
var util_save_wan5 = '无效的服务器域名！正确的域名由字母或数字或-组成且不含有空格.';
var util_save_wan6 = '无效的服务器域名! 域名应该以字母开始，以字母或数字结束.';
var util_save_wan7 = 'PPP用户名不能为空!';
var util_save_wan8 = 'PPP密码不能为空!';
var util_save_wan9 = '你不能输入非数字字符!';
var util_save_wan10 = '无效空闲时间值，必须设置为 1-1000.';
var util_save_wan11 = '无效的IP输入';
var util_save_wan12 = '2 PPP用户名不能为空!';
var util_save_wan13 = '2 PPP密码不能为空!';
var util_save_wan14 = '3 PPP用户名不能为空!';
var util_save_wan15 = '3 PPP密码不能为空!';
var util_save_wan16 = '4 PPP用户名不能为空!';
var util_save_wan17 = '4 PPP密码不能为空!';
var util_save_wan18 = '域名不能为空';
var util_save_wan19 = 'APN名不能为空!';
var util_save_wan20 = '拨号号码不能为空!';
var util_save_wan21 = 'DNS1地址不能为空!';
var util_save_wan22 = 'DNS1地址不能是0.0.0.0!';




/***********	status.htm ************/
var status_ip = 'IP 地址';
var status_mac = 'MAC 地址';
var status_subnet_mask = '子网掩码';
var status_default_gw = '默认网关';
var status_attain_ip = '获取IP协议';
var status_ipv6_global_ip = '全局地址';
var status_ipv6_ll = '链路本地地址';
var status_ipv6_link = '链路类型';
var status_ipv6_conn = '连接类型';
var status_header = '接入点状态';
var status_explain = ' 该页面显示了目前状态和设备的一些基本设置。';
var status_wan_config = '广域网络配置';
var status_ipv6_lan = '局域网络的IPV6配置';
var status_ipv6_wan = '广域网络的IPV6配置';
var status_sys = '系统';
var status_uptime = '更新时间';
var status_fw_ver = '固件版本';
var status_build_time = '构建时间';
var status_wl = '无线';
var status_config = '配置';
var status_client_mode_inf = '基础类型客户端';
var status_client_mode_adhoc = '自组网客户';
var status_ap = '接入点';
var status_wds = '无线分布式系统';
var status_ap_wds = '接入点+无线分布式系统';
var status_mesh = '网状';
var status_ap_mesh = '接入点+网状';
var status_mpp = '网状入口节点';
var status_ap_mpp = '接入点+网状入口节点';
var status_map = '网状接入点';
var status_mp = '网状节点';
var status_band = '频段';
var status_ssid = 'SSID';
var status_channel_num = '信道编号';
var status_encrypt = '加密方式';
var status_bssid = 'BSSID';
var status_assoc_cli = '已连接客户端';
var status_state = '状态';
var status_vir_ap  = '虚拟接入点';
var status_repater_config = " 中继接口配置";
var status_tcpip_config = 'TCP/IP 配置';
var status_dhcp_server = 'DHCP 服务器';
var status_disabled = '禁用';
var status_enabled = '启用';
var status_auto = '自动';
var status_unknown = '未知';
var status_dhcp_get_ip = '从DHCP服务器获取IP...';
var status_conn = '已连接';
var status_disconn = '已断开连接';
var status_fixed = '固定IP';
var status_start = '已启动';
var status_idle = '空闲';
var status_wait_key = '等待密钥';
var status_scan = '扫描';
var status_mode = '模式';
var status_access_point = '接入点状态';
var status_page_information = '本页显示设备的当前状态和和一些基本设置';
var status_version = '固件版本:';
var status_wireless_repater_config = '无线中继接口配置';
var status_wireless_config = '无线配置';
var status_wan1_config = '广域网1配置';
var status_wan2_config = '广域网2配置';
var status_wan3_config = '广域网3配置';
var status_wan4_config = '广域网4配置';
var status_dns_server = '域名服务器';
var status_wireless_text='无线 ';
var status_Configuration_text='配置';
var status_current_setting_noeffect = '下面的状态显示当前设置, 但没有生效.';



/***********	stats.htm ************/
var stats_header = '统计';
var stats_explain = '该页面显示了以太网与无线网络的传输与接收封包计数';
var stats_lan = '局域网';
var stats_send = "已发送封包数";
var stats_recv = "已接收封包数";
var stats_repeater = '中继';
var stats_eth = '以太';
var stats_wan = '广域网';
var stats_refresh = '刷新';
var stats_wireless_text = '无线 ';
var stats_ethernet = '以太网 ';
var stats_wireless_repeater = '无线中继器';
var stats_virtual_ap = '虚拟接入点 ';



/***********	wlwdstbl.htm ************/
var wlwdstbl_header = '无线分布式系统接入点列表';
var wlwdstbl_header_for_double='无线分布式系统接入点列表-wlan';
var wlwdstbl_wlan = "无线局域网";
var wlwdstbl_explain = '该表显示了每个已配置无线分布式系统接入点的MAC地址、传输与接收封包计数及状态信息。';
var wlwdstbl_mac = 'MAC地址';
var wlwdstbl_tx_pkt = '传输封包数';
var wlwdstbl_tx_err = '传输错误数';
var wlwdstbl_tx_rate = '传输速率(Mbps)';
var wlwdstbl_rx_pkt = '接收封包数';
var wlwdstbl_refresh = '刷新';
var wlwdstbl_close = '关闭';
/***********	wlwdsenp.htm ************/
var wlwdsenp_hex = '十六进制';
var wlwdsenp_char = '字符';
var wlwdsenp_header = '无线分布式系统安全设置';
var wlwdsenp_header_for_double='无线分布式系统安全设置-wlan';
var wlwdsenp_wlan = '无线局域网';
var wlwdsenp_explain = '该页面可以配置无线分布系统的安全设置。当安全设置使能时，必须确保无线分布系统内的设备已经采用同样的加密算法和密钥';
var wlwdsenp_wep_key_format = 'WEP密钥格式:';
var wlwdsenp_wep_key_ascii_5characters='ASCII(5个字符)';
var wlwdsenp_wep_key_hex_10characters='16进制（10个字符）';
var wlwdsenp_encrypt = '加密方式:';
var wlwdsenp_wep_key = 'WEP密钥:';
var wlwdsenp_prekey_format = '预共享密钥格式:';
var wlwdsenp_prekey = '预共享密钥:';
var wlwdsenp_none = 'Open';
var wlwdsenp_pass = '口令';
var wlwdsenp_hex_64characters='16进制（64个字符）';
var wlwdsenp_bits = '比特';
var wlwdsenp_apply = "设置";
var wlwdsenp_reset = '重置';
var wlwdsenp_save='保存';
var wlwdsenp_save_apply='保存&生效';

/***********	tcpip_staticdhcp.htm ************/
var tcpip_dhcp_alert1 = '无效的IP地址值!';
var tcpip_dhcp_alert2 = 'MAC地址或端口号无效。MAC地址必须是12个16进制数，端口号必须是1~5。';
var tcpip_dhcp_alert3 = '无效的IP地址值! 它和局域网IP地址一样!';
var tcpip_dhcp_alert4 = '输入的MAC地址不完整，MAC地址必须是12个16进制的长度.';
var tcpip_dhcp_alert5 = 'MAC地址无效!';

var tcpip_dhcp_del_select = '确定删除已选择的表项?';
var tcpip_dhcp_del_all = '确定删除所有表项?';
var tcpip_dhcp_header = '静态DHCP设置';
var tcpip_dhcp_explain = '该页面可以预留IP地址，允许一个拥有特殊MAC地址的网络设备在任何需要得一个IP地址的时候，都可得到同样的IP地址。这相当于一个设备拥有一个静态IP地址，除非设备必须需要从DHCP服务器获取IP地址。';
var tcpip_dhcp_st_enabled = '启用静态DHCP';
var tcpip_dhcp_ipaddr = 'IP地址:';
var tcpip_dhcp_ip = 'IP地址';

var tcpip_dhcp_mac_port = '<td align="left" width="20%" class=""><font size=2><b>MAC地址/端口号:</td>';
var tcpip_dhcp_mac_port2 = 'MAC地址/端口号:';
var tcpip_dhcp_macport ='MAC地址/端口号';

var tcpip_dhcp_mac_addr = '<td align="left" width="20%" class=""><font size=2><b>MAC地址:</td>';
var tcpip_dhcp_mac_addr2 = 'MAC地址';

var tcpip_dhcp_comment = '描述';
var tcpip_dhcp_list = '静态DHCP列表:';
var tcpip_dhcp_apply = '设置';
var tcpip_dhcp_reset = '重置';
var tcpip_dhcp_save = '保存';
var tcpip_dhcp_save_apply = '保存&生效';


var tcpip_dhcp_delsel = '删除已选';
var tcpip_dhcp_delall = '删除所有';
var tcpip_dhcp_select = '选择';

/***********	wizard.htm ************/
var wizard_header = '设置安装向导';
var wizard_header_explain = '安装向导将指导您配置为AP,请按照安装向导指示一步一步执行.';
var wizard_welcome = '欢迎来到安装向导.';
var wizard_content_explain = '该向导将引导您通过下列步骤。首先点击下一个.';
var wizard_content1 = '模式设置';
var wizard_content2 = '选择你的时区';
var wizard_content3 = '设置LAN接口';
var wizard_content4 = '设置WAN时区';
var wizard_content5 = '选择无线频率';
var wizard_content6 = '无线局域网设置';
var wizard_content7 = '无线安全性设置';
var wizard_content8 ='广域网络接口设置';
var wizard_content9 ='无线接口设置';
var wizard_content10 ='选择无线安全模式';
var wizard_op_mode = '操作模式';
var wizard_op_mode_explain ='你可以为NAT和桥接设置不同的模式到LAN和WLAN接口。 ';
var wizard_op_gw_explain= '在该模式下，设备通过ADSL或者调制解调器连接到因特网。NAT是使能的，并且局域网端口的主机通过广域网端口共享同一个IP连接到因特网的服务提供商。在广域网页面下可以设置不同的连接类型，类型包括了PPPOE，DHCP客户端，PPTP客户端，L2TP客户端，静态IP。';
var wizard_op_br_explain='在该模式下，所有的以太网端口和无线接口桥接在一起，且NAT功能是不使能的。该模式同样不支持所有与广域网端口有关的功能及防火墙';
var wizard_op_wisp='无线网络服务提供商:';
var wizard_op_wisp_explain= '在该模式下，所有的以太网端口桥接在一起，无线客户端连接到网络服务提供商的接入点。NAT是使能的，且以太网端口的主机通过无线局域网共享同一个IP连接到网络服务提供商。在网络扫描页面下可以连接到网络服务提供商的接入点。在广域网页面下可以设置不同的连接类型，类型包括了PPPOE，DHCP客户端，PPTP客户端，L2TP客户端，静态IP。';
var wizard_op_wisp_wanif = 'WAN接口: ';
var wizard_timeZone='时区设置';
var wizard_ntp_explain = '你可以通过与Internet上的时间服务器进行同步来维护系统时间.';
var wizard_ntp_enable_clientup = '启动NTP客户端更新 ';
var wizard_ntp_adjust_daylight = '自动夏令时调整 ';
var wizard_ntp_time_zone = '时区选择 :';
var wizard_ntp_server = 'NTP服务器 :';
var wizard_ntp_server_Europe1 = '131.188.3.220  - 欧洲';
var wizard_ntp_server_Europe2 = '130.149.17.8   - 欧洲';
var wizard_ntp_server_Asia = '203.117.180.36  - 亚太地区';
var wizard_lan = '局域网接口设置';
var wizard_lan_explain = '该页面可以配置连接到接入点LAN端口的局域网网络参数，配置IP地址，子网掩码，DHCP等参数';
var wizard_lan_ip = "IP地址:";
var wizard_lan_mask = '子网掩码:';
var wizard_lan_default_gateway ='默认网关:';
var wizard_wan = '广域网络接口设置';
var wizard_wan_explain = '该页面可以设置因特网的网络参数，因特网是通过接入点的WAN接口连接的。通过改变WAN接入类型可以改变为静态IP、DHCP、PPPOE、PPTP或者L2TP等不同的接入方式。'; 
var wizard_wan_access_type = 'WAN 接入类型:';
var wizard_wan1_enable = '开启 WAN1';
var wizard_static_ip = '静态 IP';
var wizard_dhcp = 'DHCP 客户端';
var wizard_username = '用户名:';
var wizard_password = '密码:';
var wizard_not_start = '未开始';
var wizard_wan_dynamic_ip = '动态IP (DHCP)';
var wizard_wan_static_ip = '静态IP';
var wizard_wan_server_ip = '服务器IP地址:';
var wizard_dial_num = '拨号号码:';
var wizard_wan_auto_dns = '自动获取DNS';
var wizard_wan_manually_dns =  '手动设置DNS';
var wizard_wire_con = '2.4G+5G  同时存在';

var wizard_next_btn = '  前进>>';
var wizard_back_btn = '<<后退  ';
var wizard_cancel_btn = '  取消  ';
var wizard_finish_btn = '完成';
var wizard_gw = '网关:';
var wizard_br ='桥接:';

var wizard_invalid_ip = '无效的IP值!';
var wizard_opmode_invalid = '无效的模式值 ';
var wizard_chanset_wrong = '错误域输入!';
var wizard_wantypeselect = '错误! 你的浏览器必须支持CSS !';
var wizard_no_wlan = '你没有wlan';
var wizard_weplen_error = '无效的WEP密匙长度';

var wizard_content5_explain = '你可以选择无线频率';
var wizard_wire_band = '无线频率:';
var wizard_wire = '无线';
var wizard_wire_basic = '基本设置';
var wizard_disabled = '禁用无线网络接口';
var wizard_band= '频段:';
var wizard_network_type = '网络类型:';
var wizard_infrastructure = "基础模式";
var wizard_adhoc = "自组网模式";
var wizard_channelwidth = '信道带宽:';
var wizard_ctlsideband = '控制边带:';
var wizard_ctlsidelowermode = '低';
var wizard_ctlsideuppermode = '高';
var wizard_chnnelnum = '信道编号:';
var wizard_enable_mac_clone = '启用MAC地址克隆 (单一以太网客户端)';
var wizard_connect_addto_profile='添加至无线配置';
var wizard_security = '安全设置';
var wizard_security_explain = '这个页面允许你设置无线安全性。使用加密密钥打开WEP或WPA，可以防止任何未经授权的访问您的无线网络。';
var wizard_encryption2 = '加密:';
var wizard_none ='Open';
var wizard_wap_mix ='WPA-Mixed';
var wizard_key_length='密钥长度';
var wizard_64_bit='64位';
var wizard_128_bit='128位';
var wizard_key_format='密钥格式';
var wizard_key_setting='密钥设置:';
var wizard_wpa_unicast = 'WPA 单播密码组:';
var wizard_psk_format='预共享密钥格式:';
var wizard_passphrase='口令';
var wizard_hex_64_characters='16进制(64个字符)';
var wizard_pre_shared_key='预共享密钥：';
var wizard_authmode = '验证模式:';
var wizard_authmode_enter_server = '企业级(作为服务器)';
var wizard_authmode_personal = '个人 (共享密匙)';
var wizard_as_ser_ipaddr = 'AS 服务器;IP 地址:';
var wizard_select_wapi_ca = '选择WAPI证书:';
var wizard_use_ca_from_as0 = '使用从远程认证服务器获取的证书0';
var wizard_use_ca_from_as1 = '使用从远程认证服务器获取的证书1';
var wizard_select_band = '你可以选择无线频段.';

var wizard_basic_header_explain = ' 本页面可以配置连接到接入点的无线客户端参数. ';
var wizard_wlan1_div0_mode = '模式:';
var wizard_chan_auto = '自动';
var wizard_client = '客户端';


var wizard_wpa_tkip = 'WPA (TKIP)';
var wizard_wpa_aes = 'WPA (AES)';
var wizard_wpa2_aes = 'WPA2(AES)';
var wizard_wpa2_mixed = 'WPA2混合';
var wizard_use_cert_from_remote_as0 = ' 用户证书来自远端AS0 ';
var wizard_use_cert_from_remote_as1 = ' 用户证书来自远端AS1 ';

var wizard_5G_basic = '无线5GHz基本设置';
var wizard_5G_sec = '无线5GHz安全设置';
var wizard_2G_basic = '无线2.4GHz基本设置';
var wizard_2G_sec = '无线2.4G安全设置';
var wizard_SSID = '无线网络名称(SSID):';
var wizard_SSID1 = ':<br>无线 1 网络名称(SSID):';
var wizard_SSID2 = ':<br>无线 2 网络名称(SSID):';
var wizard_encryption = ':<br> 加密';
var wizard_encryp = '加密';
var wizard_ulinker_mode = 'ULinker 模式 :';
var wizard_enter_wire_name = '你可以输入接入点的无线网络名称.';
var wizard_asii1 = 'ASCII (5个字符)';
var wizard_hex1 = 'Hex (10个字符)';
var wizard_asii2 = 'ASCII (13个字符)';
var wizard_hex2 = 'Hex (26个字符)';

var wizard_asii1 = 'ASCII (5个字符)';
var wizard_hex1 = 'Hex (10个字符)';
var wizard_asii2 = 'ASCII (13个字符)';
var wizard_hex2 = 'Hex (26个字符)';

/***********	route.htm ************/
var route_header = '路由设置';
var route_header_explain = '这个界面用来设置动态路由协议或设置静态路由表.';
var route_enable_static ='启用静态路由';
var route_enable = '启用';
var route_disable = '禁用';
var route_apply = '设置';
var route_reset = '重置';
var route_save = '保存';
var route_save_apply = '保存&生效';
var route_reset = '重置';

var route_dynamic = '启用动态路由';
var route_nat = 'NAT:';
var route_rip = 'RIP:';
var route_rip1 = 'RIPv1' ;
var route_rip2 = 'RIPv2';
var route_rip6 = 'RIPng';
var route_static = '启用静态路由';
var route_ipaddr = 'IP地址:';
var route_mask = '子网掩码:';
var route_gateway = '网关:';
var route_metric = '跳数:';
var route_interface = '接口:';
var route_lan = '无线局域网';
var route_wan = '无线广域网';
var route_showtbl = '显示路由表';


var route_static_tbl = '静态路由表:';
var route_static_tbl6 ='静态IPv6路由表:';
var route_tbl_destip = '目的IP地址';
var route_tbl_destipv6ip ='目的IPv6地址';
var route_tbl_mask = '掩码 ';
var route_tbl_gateway = '网关 ';
var route_tbl_metric = '跳数 ';
var route_tbl_inter = '接口 ';
var route_tbl_status = '状态';
var route_tbl_select = '选择';
var route_tbl_enable = '开启';
var route_tbl_disable = '关闭';
var route_deletechick_warn = '确定删除所选入口项?';
var route_deleteall_warn = '确定删除所有入口项?';
var route_deletechick = '删除所选';
var route_deleteall = '删除所有';
var route_showtbl = '显示路由表';

var route_addchick0 = '无效的IP地址 ';
var route_addchick1 = '无效的IP地址! ';
var route_addchick2 = '无效的网关地址! ';
var route_addchick3 = '无效的跳数! 跳数的范围为 1 ~ 15';
var route_checkip1 = 'IP地址不能为空! IP地址为如 xxx.xxx.xxx.xxx所示的4个十进制数.';
var route_checkip2 = '值,必须为十进制数 (0-9).';
var route_checkip3 = '范围在地址第1位,必须为1-223.';
var route_checkip4 = '范围在地址第2位,必须为0-255.';
var route_checkip5 = '范围在地址第3位,必须为0-255.';
var route_checkip6 = '范围在地址第4位,必须为0-255.';
var route_validnum = '无效的值,必须为十进制数(0-9).';
var route_setrip = '在PPP广域网模式下不能禁用NAT!';

var route_duplicate = '重复';

var route_ipv6_enable = '启用IPv6静态路由';
var route_ipv6_addr = 'IPv6地址:';
var route_ipv6_pre ='前缀长度';
var route_ipv6_gw ='IPv6网关:';
var route_invalid_dp = '无效的目的地址前缀!';
var route_invalid_ng = '无效的下一跳网关!';
var route_invalid_netmask = '无效的网络掩码:';
var route_invalid_netmask_nomatch = '网络掩码跟路由地址不匹配!';
var route_duplicate = '重复于条目';
var route_error = '设置路由重复\n';
var route_change ='成功改变设置!<br><br>在此期间请不要关闭或重启路由器.';

var route_invalid_netmask = '无效的网络掩码:';
var route_invalid_netmask_nomatch = '网络掩码跟路由地址不匹配!';
var route_duplicate = '重复于条目';
var route_error = '设置路由重复\n';
var route_change ='成功改变设置!<br><br>在此期间请不要关闭或重启路由器.';


/***********	routetbl.htm ************/
var routetbl_header = '路由表';
var routetbl_header_explain = '这个表显示所有的RIP(v1/v2)路由表项.';
var routetbl_refresh = '刷新';
var routetbl_close = ' 关闭 ';
var routetbl_dst = '目的IP';
var routetbl_gw = '网关';
var routetbl_mask = '掩码';
var routetbl_flag = '标记';
var routetbl_iface = '接口';
var routetbl_type = '类型';
var routetbl_metric = '路由距离';
var routetbl_under_explain = '这个表显示所有的RIPng路由表项.';
var routetbl_nhop = '下一跳';
var routetbl_ref = '引用次数';
var routetbl_use = '查找次数';
var routetbl_if = '输出接口';
var routetbl_static = '静态';
var routetbl_dynamic = '动态';


/***********	tr069config.htm************/
var tr069_header = 'TR-069 配置';
var tr069_header_explain = '这个页面用于配置 tr069 CPE。在这里你可以更改ACS的参数设置。';
var tr069_enable = '开启';
var tr069_disable = '关闭';
var tr069_url = '网址';
var tr069_username = '用户名：';
var tr069_password = '密码：';
var tr069_periodic_inform_enable = '定期通知启用：';
var tr069_periodic_inform_interval = '定期通知间隔：';
var tr069_connection_request = '连接请求';
var tr069_path = '路径：';
var tr069_port = '端口：';
var tr069_STUN_connection ='STUN 连接';
var tr069_STUN_server_url = 'STUN 服务器网址：';
var tr069_STUN_server_port = 'STUN 服务器端口：';
var tr069_save = '保存';
var tr069_save_apply = '保存 & 应用';
var tr069_undo = '撤销';
var tr069_select_file = '请选择一个文件!';
var tr069_ACS_url_not_empty = 'ACS 网址不能为空！';
var tr069_invalid_username = '非法的用户名！';
var tr069_invalid_password = '非法的密码！';
var tr069_periodic_interval_time ='请输入定期间隔的时间。';
var tr069_interval_decimal_number ='间隔应该是个十进制数 (0-9)。';
var tr069_invalid_path ='非法路径！';
var tr069_input_port_number = '请输入需要连接请求的端口号。';
var tr069_invalid_port_number = '用于连接请求的端口号非法。它应该在 0-65535之间。(0 意味着STUN默认端口号3478)';
var tr069_STUN_server_url_not_empty = 'STUN服务器网址不能为空';
var tr069_STUN_input_port_number ='请输入STUN服务器的端口号。';
var tr069_STUN_invalid_port_number ='STUN服务器端口号非法。它应该在0-65535之间。';

/**********fmtr069.c************/
var tr069_certificat = '认证';
var tr069_CA_certificat = 'CA 认证:';
var tr069_upload = '上传';
var tr069_ACS_certificat = 'ACS 认证 CPE：';
var tr069_certificat_management = '认证管理：';
var tr069_authentication = '认证';

/**********fmget.c************/
var fmget_ip_dhcp = '从DHCP服务器获取IP...';
var fmget_ip = '静态IP';
var fmget_dhcp = 'DHCP';
var fmget_ip_connect = '静态IP已连接';
var fmget_ip_disconnect = '静态IP未连接';
var fmget_pppoe_connet = 'PPPoE已连接';
var fmget_pppoe_disconnect = 'PPPoE未连接';
var fmget_pptp_connect = 'PPTP已连接';
var fmget_pptp_disconnect = 'PPTP未连接';
var fmget_l2tp_connect = 'L2TP已连接';
var fmget_l2tp_disconnect = 'L2TP未连接';

/*********fmfwall.c*************/
var fmfwall_not_set_ip_addr = '错误！没有设置IP地址。';
var fmfwall_invalid_ip_addr = '无效的IP地址！必须设在当前子网下.';
var fmfwall_enable = '开启';
var fmfwall_disable = '关闭';
var fmfwall_guarantee_min_bandwidth = '保护最小带宽';
var fmfwall_restrict_max_bandwidth = '限制最大带宽';
var fmfwall_set_mode = '设置过滤模式错误！';
var fmfwall_ip_filter_type = '错误！协议类型不匹配.';
var fmfwall_miss_addr = '错误！IP地址设置不完整.';
var fmfwall_invalid_protocol_type = '错误！协议类型不支持.';
var fmfwall_protocol_not_empty = '错误！协议类型不能为空.';
var fmfwall_rule_exist = '过滤条件已经存在！';


/**********cluster_mgmt.asp************/
var cluster_header = '集群管理';
var cluster_header_explain = '集群管理是一种将你的WLAN配置到LAN/WAN接入点的服务。如果开启，所有支持集群管理的LAN/WAN接入点将共享相同的WLAN配置。';
var cluster_management = '开启集群管理';
var cluster_save_apply = '保存 & 应用';
var cluster_reset = '复位';
var cluster_management_not_support ='当前固件不支持集群管理！';

/***********	vlan.htm ************/
var vlan_header = '虚拟局域网设置';
var vlan_header_explain = '这个界面用来进行虚拟局域网配置,路由器通过创建虚拟局域网来提供细分服务,虚拟局域网用来解决可伸缩性、安全性和网络管理等问题.';
var vlan_apply = '设置';
var vlan_reset = '重置';
var vlan_save_apply = '保存&设置';
var vlan_save = '保存';

var vlan_enable = '启用虚拟局域网';
var vlan_enable2 = '开启';
var vlan_ethe_wire = '有线/无线';
var vlan_vid ='虚拟局域网 ID';
var vlan_pri ='优先级';
var vlan_id = '虚拟局域网 ID:';
var vlan_forwardrule = '转发规则:';
var vlan_forwardrulenat = '网络地址转换';
var vlan_forwardrulebridge = '桥接模式';
var vlan_tagtbl = 'Tag表';
var vlan_tagtbl_interface = '接口名';
var vlan_tagtbl_taged = 'Tagged';
var vlan_tagtbl_untaged = 'unTagged';
var vlan_tagtbl_notin = '不在这个虚拟局域网中';

var vlan_settbl = '当前无线局域网设置';
var vlan_settbl_id = '虚拟局域网ID';
var vlan_settbl_taged = 'taged接口';
var vlan_settbl_untaged = 'untaged接口';
var vlan_settbl_forwardrule = '转发规则';
var vlan_settbl_modify = '修改';
var vlan_settbl_select = '选择';
var vlan_deletechick = '删除所选';
var vlan_deleteall = '删除所有';

var vlan_nettbl = '当前网络接口设置';
var vlan_nettbl_name = '接口名';
var vlan_nettbl_pvid = '端口VLAN标识号';
var vlan_nettbl_defprio = '默认优先级';
var vlan_nettbl_defcfi = '默认Cfi';

var vlan_checkadd1 = '无效的虚拟局域网id,虚拟局域网的id的值必须在1~4094之间';
var vlan_checkadd2 = '这个虚拟局域网必须绑定至少1个接口!!';
var vlan_deletesel = '确定删除所选表项?';
var vlan_deleteall_conf = '确定删除所有表项?';

/***********	8021q_vlan.htm ************/
var vlan_8021q_header = '802.1Q虚拟局域网';
var vlan_8021q_header_explain = '这个界面用来进行虚拟局域网配置，路由器通过创建虚拟局域网来提供细分服务，虚拟局域网用来解决可伸缩性、安全性和网络管理等问题.';
var vlan_8021q_apply = '设置';
var vlan_8021q_reset = '重置';
var vlan_8021q_save = '保存';
var vlan_8021q_save_apply ='保存 & 设置';


var vlan_8021q_del_select = '删除所选表项?';
var vlan_8021q_del_all = '删除所有表项?';
var vlan_8021q_forwardrule = '转发规则:';
var vlan_8021q_forwardrulenat = '网络地址转换';
var vlan_8021q_forwardrulebridge = '桥接模式';
var vlan_8021q_hw_nat = '硬件网络地址转换:';
var vlan_8021q_enable = '启用802.1Q虚拟局域网';
var vlan_8021q_id ='虚拟局域网ID(1-4095):';
var vlan_8021q_pri = '优先级(0-7):';
var vlan_8021q_port = '端口';
var vlan_8021q_member = '成员';
var vlan_8021q_taged = 'Tagged';
var vlan_8021q_settbl ='当前无线局域网表:';

var vlan_8021q_vid = '虚拟局域网ID';
var vlan_8021q_priority = '优先级';
var vlan_8021q_forrule = '转发规则';
var vlan_8021q_tagport = 'Tagged端口';
var vlan_8021q_untagport = 'Untagged端口';
var vlan_8021q_select = '选择';
var vlan_8021q_deletechick = '删除所选';
var vlan_8021q_deleteall = '删除所有';
var vlan_8021q_change_pvid = '改变端口虚拟局域网标识号的设置';
var vlan_8021q_def_vid = '默认虚拟局域网ID';

var vlan_8021q_nathw = '网络地址转换(硬件)';
var vlan_8021q_nat = '网络地址转换';
var vlan_8021q_br = '桥接';
var vlan_8021q_unknown = '未知';
var vlan_8021q_error1 = '错误! 无效的转发规则!';
var vlan_8021q_error2 = '无效的虚拟局域网ID!';
var vlan_8021q_error3 = '错误! 无效的优先级!';
var vlan_8021q_error4 = '没有选择端口!';
var vlan_8021q_error5 = '必须包含WAN和至少一个LAN/WLAN口!';
var vlan_8021q_error6 = '当转发规则是网络地址转换时,WAN和LAN/WLAN口应该放在不同的虚拟局域网组中!';
var vlan_8021q_error7 = '最多只支持一个untagged的Bridge-WAN!';
var vlan_8021q_error8 = 'LAN/WLAN口最多只能放在一个组中!';
var vlan_8021q_error9 = '硬件网络地址转换只为了NAT-LAN组!';
var vlan_8021q_error10 = '只有一个NAT-LAN组可以硬件转发!';
var vlan_8021q_error11 = '错误! 无效的PVID值!';
var vlan_8021q_error12 ='当转发规则是桥接时, WAN和至少一个LAN/WLAN端口都应该被包含!';
var vlan_8021q_error13 ='端口应该都是tagged或者untagged!';
var vlan_8021q_error14 = '设置了重复的VID!';
var vlan_8021q_error15 = '只能存在一个NAT!';
var vlan_8021q_error16 = '当转发规则是网络地址转换时, 包含WAN的VLAN组只能存在一个!';
var vlan_8021q_vid2 = '虚拟局域网id';
var vlan_8021q_vid_reserve  = '和100是做为内部使用!';
var vlan_8021q_max_table = '最大虚拟局域网条目数是';




/***********	vlan_wan.htm ************/
var vlan_wan_header = '会话初始协议';
var vlan_wan_header_explain = '为WAN端口的封包添加虚拟局域网Tags';
var vlan_wan_enable_tag = 'WAN端口封包启用Tag';
var vlan_wan_all_ptk = '对于所有封包';
var vlan_wan_id = '虚拟局域网ID';
var vlan_wan_host_pkt = '对于到本地主机的封包';
var vlan_wan_local_pkt = '对于本地封包';
var vlan_wan_pri = '优先级';
var vlan_wan_wifi_root_ptk = '对于根无线网的封包';
var vlan_wan_wifi_vap1_ptk = '对于无线虚拟接入点1的封包';
var vlan_wan_wifi_vap2_ptk = '对于无线虚拟接入点2的封包';
var vlan_wan_wifi_vap3_ptk = '对于无线虚拟接入点3的封包';
var vlan_wan_wifi_vap4_ptk = '对于无线虚拟接入点4的封包';
var vlan_wan_bridge = '和WAN做桥接';
var vlan_wan_multicast = '多播包';
var vlan_wan_lan0 = 'LAN端口0';
var vlan_wan_lan1 = 'LAN端口1';
var vlan_wan_lan2 = 'LAN端口2';
var vlan_wan_lan3 = 'LAN端口3';
var vlan_wan_wifi_root = '根无线网';
var vlan_wan_vap1 = '无线虚拟接入点1';
var vlan_wan_vap2 = '无线虚拟接入点2';
var vlan_wan_vap3 = '无线虚拟接入点3';
var vlan_wan_vap4 = '无线虚拟接入点4';
var vlan_wan_note1 = '注意I: lan端口0是靠近WAN端口的.';
var vlan_wan_note2 = '注意II: 虚拟局域网ID 9是为其他目的而保留的ID.';
var vlan_wan_apply = '设置';
var vlan_wan_reset = '重置';
var vlan_wan_save = '保存';
var vlan_wan_save_apply ='保存 & 设置';


/***********	wlmultipleap.htm ************/
var wlmultipleap_header="虚拟AP";
var wlmultipleap_explain="这个页面显示并更新虚拟AP的无线设置";

var wlmultiple_no="序号";
var wlmultiple_enable="启用";
var wlmultipleap_band="频段";
var wlmultiple_SSID=" SSID";
var wlmultiple_data_rate="数据速率";
var wlmultiple_ssid_priority = 'SSID优先级';
var wlmultiple_brocadcast_ssid="广播SSID";
var wlmultiple_wmm="无线多媒体";
var wlmultiple_access="接入控制";
var wlmultiple_tx_restrict="发送速率限制(Mbps)";
var wlmultiple_rx_restrict="接收速率限制(Mbps)";
var wlmultiple_active_client="活跃客户端列表";
var wlmultiple_wlan_mode="无线模式";
var wlmultipleap_reset = '重置';
var wlmultiple_save = '保存';
var wlmultiple_save_apply = '保存&生效';

/***********wlsecurity_all.htm ************/
var wlsecurity_ascii_5characters='ASCII(5个字符)';
var wlsecurity_hex_10characters='16进制(10个字符)';
var wlsecurity_ascii_13characters='ASCII (13个字符)';
var wlsecurity_hex_26characters='16进制(26个字符)';
var wlsecurity_encryption='加密方式:';
var wlsecurity_disable='Open';
var wlsecurity_8021_auth='802.1x认证:';
var wlsecurity_authentication='认证方式:';
var wlsecurity_open_system='开放系统';
var wlsecurity_shared_key='共享密钥';
var wlsecurity_auto='自动';
var wlsecurity_key_length='密钥长度';
var wlsecurity_64_bit='64位';
var wlsecurit_128_bit='128位';
var wlsecurity_key_format='密钥格式';
var wlsecurity_encryption_key='加密密钥';
var wlsecurity_authentication_mode='认证方式';
var wlsecurity_enterprise_radius='企业RADIUS';
var wlsecurity_psk='个人预共享密钥';
var wlsecurity_wpa_cipher_suite='WPA密码组:';
var wlsecurity_tkip='TKIP';
var wlsecurity_aes='AES';
var wlsecurity_wpa2_cipher_suite='WPA2密码组:';
var wlsecurity_management_frame_protection='管理帧保护：';
var wlsecurity_none='无';
var wlsecurity_capable='可选';
var wlsecurity_required='必须';
var wlsecurity_sha256='SHA256:';
var wlsecurity_sha256_disable='关闭';
var wlsecurity_sha256_enable='开启';
var wlsecurity_psk_format='预共享密钥格式:';
var wlsecurity_passphrase='口令';
var wlsecurity_hex_64_characters='16进制(64个字符)';
var wlsecurity_pre_shared_key='预共享密钥：';
var wlsecurity_radius_server_ip_address='RADIUS服务器IP地址:';
var wlsecurity_radius_server_port='RADIUS服务器端口:';
var wlsecurity_radius_server_password='RADIUS服务器密码:';
var wlsecurity_encryption_is_none="此时未进行加密";
var wlsecurity_wep_on_wps20_disable="设置为WEP加密方式，WPS2.0将被禁用";
var wlsecurity_8021x_eap='不支持802.1x eap!'
var wlsecurity_8021x_eap_inside='不支持802.1x eap内部类型!'

/***********wlmesh.htm ************/
var wlmesh_header='无线多跳网络设置'
var wlmesh_explain='多跳网络像以太网一样使用无线媒介与其他无线接入点进行通信，如果使用此功能，您必须将AP设置在同一信道并具有相同的多跳网络ID，无线接入点必须在无线接入点+多跳网络或多跳网络模式下。'
var wlmesh_enable_mesh='启动多跳网络'
var wlmesh_mesh_id='多跳网络ID:'
var wlmesh_encryption='加密方式:'
var wlmesh_encryption_none='无'
var wlmesh_psk_format='预共享密钥格式:'
var wlmesh_psk_format_passphrase='口令'
var wlmesh_psk_format_hex64cha='16进制(64字符)'
var wlmesh_psk='预共享密钥:'
var wlmesh_mesh_crossband='多跳网络交叉频带:'
var wlmesh_mesh_crossband_enable='启用'
var wlmesh_mesh_crossband_disable='禁用'
var wlmesh_save='保存'
var wlmesh_save_apply='保存&生效'
var wlmesh_reset='重置'
var wlmesh_set_access_control='设定访问控制'
var wlmesh_show_advenced_information='显示高级信息'
/***********wlmeshACL.htm ************/
var wlmeshACL_header='多跳网络访问控制列表'
var wlmeshACL_explain='如果您选择‘允许列出的’，只有无线MAC地址在访问控制列表中的多跳网络节点能够连接到多跳网络，当选择‘拒绝列出的’时，列表中的多跳网络节点将不能够创建连接。'
var wlmeshACL_mode='模式'
var wlmeshACL_mode_disable='关闭'
var wlmeshACL_mode_allow='允许列出的'
var wlmeshACL_mode_deny='拒绝列出的'
var wlmeshACL_mac_address='MAC地址'
var wlmeshACL_comment='注释'
var wlmeshACL_save='保存'
var wlmeshACL_save_apply='保存&生效'
var wlmeshACL_reset='重置'
var wlmeshACL_current_acl='当前访问控制列表:'
var wlmeshACL_mac_address_in_table='MAC地址'
var wlmeshACL_comment_in_table='注释'
var wlmeshACL_select_in_table='选择'
var wlmeshACL_delete_select_btn = '删除所选';
var wlmeshACL_delete_all_btn = '删除全部';
var wlmeshACL_input_mac_not_complete='输入的MAC地址不完整，MAC地址必须是12个16进制的长度.'
var wlmeshACL_delete_all_entries='删除所有的条目将导致所有的同级设备不能连接到MP.确定?'
var wlmeshACL_delete_selected_entry='你真的想删除选定的条目吗?'
var wlmeshACL_mac_address='MAC地址'
var wlmeshACL_comment='说明'
var wlmeshACL_select='选择'

/***********wlmeshinfo.htm ************/
var wlmeshinfo_header='无线多跳网络信息'
var wlmeshinfo_explain='这些信息仅适用于对无线多跳网络具有足够知识的高级技术用户'
var wlmeshinfo_root='主'
var wlmeshinfo_neighbor_table='邻居表'
var wlmeshinfo_mac_address='MAC地址'
var wlmeshinfo_mode='模式'
var wlmeshinfo_tx_packets='传输分组数'
var wlmeshinfo_rx_packets='接收分组数'
var wlmeshinfo_tx_rate='传输速率(Mbps)'
var wlmeshinfo_rssi='接收信号强度'
var wlmeshinfo_expired_time='过期时间(s)'
var wlmeshinfo_routing_table='路由表'
var wlmeshinfo_destination_mesh_point='目标多跳网络节点'
var wlmeshinfo_next_hop_mesh_point='下一跳多跳网络节点'
var wlmeshinfo_portal_enable='入口启用'
var wlmeshinfo_metric='度量'
var wlmeshinfo_hop_count='总跳数'
var wlmeshinfo_portal_table='入口表'
var wlmeshinfo_portal_mac='入口MAC'
var wlmeshinfo_wireless_station_list='无线设备列表'
var wlmeshinfo_mac = 'MAC地址';
var wlmeshinfo_mode = '模式';
var wlmeshinfo_tx = '传输分组数';
var wlmeshinfo_rx = '接收分组数';
var wlmeshinfo_tx_rate ='传输速率 (Mbps)';
var wlmeshinfo_ps = '节能';
var wlmeshinfo_expired_time = '过期时间(s)';
var wlmeshinfo_proxy_table='代理表'
var wlmeshinfo_owner='拥有者'
var wlmeshinfo_client='客户端'
var wlmeshinfo_refresh = '刷新';
/***********wlwapiinstallcert.htm ************/
var wlwapiinstalcert_header='证书安装'
var wlwapiinstalcert_explain='此页面允许您为我们的AP安装ASU和用户认证。请注意，目前支持X.509认证。'
var wlwapiinstalcert_select_wapi_mode='选择WAPI模式:'
var wlwapiinstalcert_two_certification='两证书'
var wlwapiinstalcert_three_certification='三证书'
var wlwapiinstalcert_certification_remote='远端鉴权服务器证书类型:'
var wlwapiinstalcert_ca_certificate_remote='远端鉴权服务器的CA证书'
var wlwapiinstalcert_asu_certificate_remote='远端鉴权服务器的ASU证书'
var wlwapiinstalcert_user_certificate_remote='远端鉴权服务器的用户证书'
var wlwapiinstalcert_x509='X.509'

var wlwapiinstalcert_certificate_local='当地鉴权服务器的证书类型'
var wlwapiinstalcert_ca_certificate_local='远端鉴权服务器的CA证书'
var wlwapiinstalcert_asu_certificate_local='远端鉴权服务器的ASU证书'
var wlwapiinstalcert_user_certificate_local='远端鉴权服务器的用户证书'
var wlwapiinstalcert_select_file='选择文件'
var wlwapiinstalcert_no_file_selected='未选择任何文件'
var wlwapiinstalcert_upload='上传'
var wlwapiinstalcert_reset='重置'
var wlwapiinstalcert_update_firmware_failed='升级固件失败!'
var wlwapiinstalcert_file_name_cannot_empty='文件名不能为空!'

/***********wlwapiRekey.htm ************/
var wlwapirekey_header='密钥更新'
var wlwapirekey_explain='此页面允许您配置组播密钥更新和单播密钥更新。 在这里，您可以关闭密钥更新，或者您可以选择基于时间，基于分组或基于时间包的密钥更新。'
var wlwapirekey_key_regroup='密钥重组'
var wlwapirekey_forced_key='强制密钥:'
var wlwapirekey_on='开'
var wlwapirekey_off='关'
var wlwapirekey_multicast_key='组播密钥'
var wlwapirekey_unicast_key='单播密钥'
var wlwapirekey_mac_address='Mac地址:'
var wlwapirekey_re_key='密钥更新'
var wlwapirekey_multicast_key_update='组播密钥更新:'
var wlwapirekey_time_choice='时间'
var wlwapirekey_packets_choice='分组数'
var wlwapirekey_time_packets_choice='时间+分组数'
var wlwapirekey_time='时间:'
var wlwapirekey_s='秒'
var wlwapirekey_byte='字节'
var wlwapirekey_packets='分组数'
var wlwapirekey_unicast_key_update='单播密钥更新:'
var wlwapirekey_save='保存'
var wlwapirekey_save_apply='保存&生效'
var wlwapirekey_reset='重置'
var wlwapirekey_input_mac_address='请输入MAC地址!'
var wlwapirekey_key_group_time='密钥重组时间应该在300 ~~ 31536000'
var wlwapirekey_number_of_packets_should_great='分组数应该大于1048576'
var wlwapirekey_number_of_packets_toobig='分组数太大'

/***********wlwapicertmanagement.htm ************/
var wlwapicertman_header='证书管理'
var wlwapicertman_explain='此页面允许您管理我们的AP颁发的证书。在这里，您可以颁发，清楚，撤销，搜索AP所颁发的任何证书。此外，您可以下载AS证书。'
var wlwapicertman_certificaiton_list='证书列表'
var wlwapicertman_selected='已选'
var wlwapicertman_owner='拥有者'
var wlwapicertman_serialno='序号'
var wlwapicertman_valid_time='有效时间'
var wlwapicertman_total='总共'
var wlwapicertman_left='剩余'
var wlwapicertman_type='类型'
var wlwapicertman_status='状态'
var wlwapicertman_selectall='选择全部'
var wlwapicertman_pageup='向上翻页'
var wlwapicertman_pagedown='向下翻页'
var wlwapicertman_revoke='撤销'
var wlwapicertman_distribute='颁发'
var wlwapicertman_search='查找'
var wlwapicertman_all='全部'
var wlwapicertman_serialno='序号'
var wlwapicertman_owner='拥有者'
var wlwapicertman_type='类型'
var wlwapicertman_status='状态'
var wlwapicertman_activated='已激活'
var wlwapicertman_revoked='已撤销'
var wlwapicertman_search='查找'
var wlwapicertman_click_download='点击此处下载CA X.509证书'
var wlwapicertman_clear_all='清除以前的证书并产生新的CA证书'
var wlwapicertman_input_serial_number='请输入序号!'
var wlwapicertman_length_serial_number_error='序号长度错误!'
var wlwapicertman_no_cert_to_revoke='对不起，没有可撤销的证书'
var wlwapicertman_select_cert_to_remove='对不起，请选择需要移除的证书'
var wlwapicertman_remove_selected_cert='移除选中的证书?'
var wlwapicertman_makesure_systime_time='在进行下一步之前，请确保我们的系统时间已经同步正确。\n我们的系统时间已经同步正确了吗？'
var wlwapicertman_clearall_cert='该步骤将清除所有颁发的证书并产生新的CA证书，您确定要进行吗？'
var wlwapicertman_no_cert_selected='对不起，没有选中的证书'
var wlwapicertman_ca_cert_isnot_ready='由于CA证书未准备就绪，请先按“清除以前的证书并产生新的CA证书”按钮来首先产生CA证书'
var wlwapicertman_ascer_isnot_ready='由于as.cer未准备就绪，请先按“清除以前的证书并产生新的CA证书”按钮来首先产生CA证书'
var wlwapicertman_cancel_cert='取消证书?'
/***********	wlwapidistribute.htm ************/
var wlwapidistribute_as_timeout='AS超时'
var wlwapidistribute_please_input_username='请输入用户名!'
var wlwapidistribute_username_max_than32='用户名长度不能超过32!'
var wlwapidistribute_input_time='请输入有效时间!'
var wlwapidistribute_time_number='时间必须为数字!'
var wlwapidistribute_valid_cant_exceed="有效时间不能超过"
var wlwapidistribute_day='天'
var wlwapidistribute_year='年'
var wlwapidistribute_reset_cur_setting='您确定要将当前设置重置为默认设置?'
var wlwapidistribute_header='保存/重新加载设置'
var wlwapidistribute_explain='此页面允许您将当前设置保存到文件或者重新加载先前保存的设置文件。此外，您可以将当前设置恢复为出厂设置'
var wlwapidistribute_type='类型'
var wlwapidistribute_x509_cert='X.509证书'
var wlwapidistribute_user_name='用户名'
var wlwapidistribute_valid_time='有效时间'

var wlwapidistribute_month='月'

var wlwapidistribute_save_certto_file='保存证书到文件:'
var wlwapidistribute_save___='保存中...'
/***********	wlbandmode.htm ************/
var wlbandmode_header='无线频段设置'
var wlbandmode_explain='支持可切换的802.11n单频或双频射频.'
var wlbandmode_2g_5g='2.4G/5G选择模式:'
var wlbandmode_this_mode_sup='该模式支持2.4G or 5G.'
var wlbandmode_2g_5g_concurrent='2.4G/5G并发模式:'
var wlbandmode_simultaneously='此模式可同时支持2.4G和5G无线网络连接.'
var wlbandmode_save='保存'
var wlbandmode_save_apply='保存&生效'
var wlbandmode_reset='重置'
/***********	wlmeshproxy.htm ************/
var wlmeshproxy_header='活跃的客户端列表'
var wlmeshproxy_explain='该表显示了每个代理的有线或无线客户端的MAC地址'
var wlmeshproxy_proxy_table='代理表'
var wlmeshproxy_refresh='刷新'
var wlmeshproxy_close='关闭'
/***********	wlp2p.htm ************/
var wlp2p_header='Wifi直连'
var wlp2p_explain='Wi-Fi直连是Wi-Fi联盟基于Wi-Fi点对点规范所提出的一套软件协议，该协议允许Wi-Fi设备在不需要无线接入点（热点）的情况下相互通信。Wi-Fi直连软件协议由Wi-Fi联盟开发并管理，Wi-Fi联盟是一个行业组织，该组织开发了Wi-Fi认证陈旭的标准套件，并拥有Wi-Fi商标。 '
var wlp2p_role_device='角色: 设备'
var wlp2p_status_listen='状态: 监听'
var wlp2p_status_disabled='状态: P2P禁用'
var wlp2p_force_group_owner='Force group owner'
var wlp2p_intent_value='整数值:'
var wlp2p_listen_channel='监听信道:'
var wlp2p_device_name='设备名称:'
var wlp2p_self_pin_number='自己的PIN码:'
var wlp2p_save='保存'
var wlp2p_save_apply='保存&生效'
var wlp2p_reset='重置'
var wlp2p_device_name_empty="设备名不能为空!!"
var wlp2p_error_creating_request='Error creating request object !'
var wlp2p_please_select_onedevice='请先选择一个设备.'
var wlp2p_scan='扫描'
var wlp2p_connect='连接'
var wlp2p_back_todevice_mode='返回到设备模式'

/***********	wlp2pwscel.htm ************/
var wlp2pwscel_pin_display='1.PIN display'
var wlp2pwscel_pin_keypad='2.PIN keypad'
var wlp2pwscel_pbc='3.PBC'
var wlp2pwscel_select_wsc='选择WSC配置方式'
/***********	wlp2pwscconnect.htm ************/
var wlp2pwscconnect_self_pin_number='自己的PIN码:'
var wlp2pwscconnect_key='Key in PIN code:'
var wlp2pwscconnect_use_pbc='Use PBC for WSC provision'
var wlp2pwscconnect_ok='确认'
/***********	wlp2pscanlist.htm ************/
var wlp2pwscconnect_device_name='设备名称'
var wlp2pwscconnect_device_address='设备地址'
var wlp2pwscconnect_channel='信道'
var wlp2pwscconnect_role='角色'
/***********	pocket_sitesurvey.htm ************/
var pocket_site_survey_running="扫描正在运行，请稍后重试。"
/************fmwlan.c*********/
var wl_Del_tab_ent="删除表格项错误!"
var wl_Add_tab_ent="添加表格项!"
var wl_set_MIB_WLAN_SCHEDULE_ENABLED_err="设置MIB_WLAN_SCHEDULE_ENABLED错误!"
var wl_set_MIB_WLAN_SCHEDULE_DELALL_err="MIB_WLAN_SCHEDULE_DELALL错误!"
var wl_set_MIB_WLAN_SCHEDULE_ADD_error="MIB_WLAN_SCHEDULE_ADD错误!"
var wl_Get_tab_ent="获得表格项错误!"
var wl_Can_add_new="不能添加新的表项，因为表已满!"
var wl_set_mesh_enable="开启mesh错误!"
var wl_set_mesh_cross="开启mesh交叉频带错误!"
var wl_set_mesh_root="开启主mesh错误!"
var wl_clear_MIB_WLAN_RS_USER_CERT_PASSWD_err="[1] 清除 MIB_WLAN_RS_USER_CERT_PASSWD 错误!"
var wl_You_can_set="当802.1x开启时模式不能设置为客户端!<br><br>请先在安全配置页面改变加密方式."
var wl_Set_wla_Mac="设置无线mac克隆错误!"
var wl_Set_MIB_WLAN_MODE_err="设置MIB_WLAN_MODE错误!"
var wl_Add_Pro_dup="重复添加配置!"
var wl_Set_SSI_err="设置SSID错误!"
var wl_Del_all_tab="删除所有表格错误!"
var wl_Set_ena_fla="设置使能标志错误!"
var wl_Set_mod_err_unknown="设置模式错误! 未知模式!!"
var wl_Set_mod_err="设置模式错误!"
var wl_Set_SSI_Bas="Set SSID Base Mode error!"
var wl_Set_SSI_Bas_empty="Set SSID Base Mode error!Empty allocation!!"
var wl_Set_SSI_Bas_alloc="Set SSID Base Mode error!allocation must be 0~100!!"
var wl_Set_SSI_Bas_total="Set SSID Base Mode error!Totoal allocation must be less than 100!!"
var wl_Err_Ple_fil_applocation="Error! Please fill allocation!"
var wl_Err_Ple_fil="错误! 请填写ip地址或mac地址!"
var wl_Inv_IP_add="无效的ip地址!ip地址应在当前子网之内."
var wl_ip_add_exi="ip地址已存在!"
var wl_Err_Inv_MAC="错误! 无效的MAC地址."
var wl_mac_add_exi="MAC地址已存在!"
var wl_Tot_All_mus="Total Allocation must be less than 100!"
var wl_Err_Com_len="错误! 注释长度太长."
var wl_Get_ent_num="获取条目数错误!"
var wl_Can_add_new_station="表已满，不能添加新的station!"
var wl_Err_no_enc="错误! 无加密方式."
var wl_Inv_enc_met="无效的加密方式!"
var wl_Set_MIB_WLAN_MESH_ENCRYPT_mib="设置MIB_WLAN_MESH_ENCRYPT mib 错误!"
var wl_Set_MIB_WLAN_MESH_AUTH_TYPE_fai="设置 MIB_WLAN_MESH_AUTH_TYPE 失败!"
var wl_Set_MIB_WLAN_MESH_WPA2_UNICIPHER_fai="设置 MIB_WLAN_MESH_WPA2_UNICIPHER 失败!"
var wl_Err_no_psk="错误! 无PSK格式."
var wl_Err_inv_psk_format="错误! 无效的PSK格式."
var wl_Set_MIB_WLAN_MESH_PSK_FORMAT_fai="设置 MIB_WLAN_MESH_PSK_FORMAT 失败!"
var wl_Err_inv_psk_value="错误! 无效的PSK值."
var wl_Set_MIB_WLAN_MESH_WPA_PSK_err="设置 MIB_WLAN_MESH_WPA_PSK 错误!"
var wl_Err_No_mac="错误! 没有要设置的mac地址."
var wl_Err_Inv_Mes="错误! 无效的Mesh ID."
var wl_Set_MIB_WLAN_MESH_ID_err="设置 MIB_WLAN_MESH_ID 错误!"
var wl_Set_MIB_WLAN_WPA2_UNICIPHER_fai="设置 MIB_WLAN_WPA2_UNICIPHER 失败!"
var wl_Set_MIB_WLAN_WPA_CIPHER_SUITE_fai="设置 MIB_WLAN_WPA_CIPHER_SUITE 失败!"
var wl_Key_len_mus="秘钥长度必须存在!"
var wl_Inv_key_len="无效的秘钥长度值!"
var wl_Set_WEP_MIB="设置 WEP MIB 错误!"
var wl_Set_MIB_WLAN_ENCRYPT_mib="设置 MIB_WLAN_ENCRYPT mib 错误!"
var wl_Key_typ_mus="密钥类型必须存在!"
var wl_Inv_key_typ="无效的秘钥类型值!"
var wl_Set_WEP_key="设置WEP 密钥类型错误!"
var wl_Inv_wep_val="无效的 wep-key 值!"
var wl_Set_wep_err="设置 wep-key 错误!"
var wl_Inv_def_tx="无效的 default tx key id!"
var wl_Set_def_tx="设置 default tx key id 错误!"
var wl_Inv_key_1="无效的 key 1 长度!"
var wl_Inv_wep1_val="无效的 wep-key1 值!"
var wl_Set_wep1_err="设置 wep-key1 错误!"
var wl_Inv_key_2="无效的 key 2 长度!"
var wl_Inv_wep2_val="无效的 wep-key2 值!"
var wl_Set_wep2_err="设置 wep-key2 错误!"
var wl_Inv_key_3="无效的 key 3 长度!"
var wl_Inv_wep3_val="无效的 wep-key3 值!"
var wl_Set_wep3_err="设置 wep-key3 错误!"
var wl_Inv_wep4_val="无效的 wep-key4 值!"
var wl_Set_wep4_err="设置 wep-key4 错误!"
var wl_Err_802_aut_wds="错误! 当设备处于wds或mesh模式时，不能使用802.1x 认证"
var wl_Err_802_aut="错误! 当设备处于客户端模式时，不能使用802.1x 认证"
var wl_Set_hs2_ena="设置 hs2 使能标志错误!"
var wl_Set_1x_ena="设置 1x 使能标志错误!"
var wl_Get_MIB_WLAN_WEP_MIB="获取 MIB_WLAN_WEP MIB 错误!"
var wl_Set_MIB_WLAN_MAC_AUTH_ENABLED_MIB="设置 MIB_WLAN_MAC_AUTH_ENABLED MIB 错误!"
var wl_Set_MIB_WLAN_IEEE80211W_fai="设置 MIB_WLAN_IEEE80211W 失败!"
var wl_Set_MIB_WLAN_SHA256_ENABLE_fai="设置 MIB_WLAN_SHA256_ENABLE 失败!"
var wl_Err_WAP_AS="错误! 当设备处于客户端模式时，不能使用WAPI AS."
var wl_Err_Inv_wap="错误! 无效的 wapi 认证值."
var wl_Set_MIB_WLAN_WAPI_AUTH_fai="设置 MIB_WLAN_WAPI_AUTH 失败!"
var wl_Set_MIB_WLAN_PSK_FORMAT_fai="设置 MIB_WLAN_PSK_FORMAT 失败!"
var wl_Err_inv_psk_len="错误! 无效的PSK长度."
var wl_Err_Set_wap="错误! 设置 wapi 密钥长度错误"
var wl_Set_MIB_WLAN_WPA_PSK_err="设置 MIB_WLAN_WPA_PSK 错误!"
var wl_No_WAP_AS="无WAPI AS 地址!"
var wl_Inv_AS_IP="无效的 AS IP地址 值!"
var wl_No_WAP_cer="未选中 WAPI 证书!"
var wl_Set_RS_IP="设置 RS IP地址错误!"
var wl_Set_WAP_cer="设置 WAPI cert sel 错误!"
var wl_Set_MIB_WLAN_ENABLE_SUPP_NONWPA_mib="设置 MIB_WLAN_ENABLE_SUPP_NONWPA mib 错误!"
var wl_Set_MIB_WLAN_SUPP_NONWPA_mib="设置 MIB_WLAN_SUPP_NONWPA mib 错误!"
var wl_Err_WPA_can_wds="错误! 当设备处于wds或mesh模式时，不能使用WPA-RADIUS."
var wl_Err_WPA_can="错误! 当设备处于客户端模式时，不能使用WPA-RADIUS."
var wl_Err_Inv_wpa="错误! 无效的 wpa 认证值."
var wl_Set_MIB_WLAN_WPA_AUTH_fai="设置 MIB_WLAN_WPA_AUTH 失败!"
var wl_Inv_val_of_cipher="无效的秘钥套件值!"
var wl_Err_Can_set="错误! 当设备处于客户端模式时，不能将密钥套件设为TKIP + AES."
var wl_Inv_val_of_80211w="IEEE80211w值无效!"
var wl_Inv_val_ofsha="wpa2EnableSHA256值无效!"
var wl_Inv_val_of_wpa2="wpa2 cipher suite值无效!"
var wl_Set_MIB_WLAN_WPA2_CIPHER_SUITE_fai="设置 MIB_WLAN_WPA2_CIPHER_SUITE 失败!"
var wl_Err_Inv_val_day="错误! rekey day值无效."
var wl_Err_Inv_val_hr="错误! rekey hr值无效."
var wl_Err_Inv_val_min="错误! rekey min值无效."
var wl_Err_Inv_val_sec="错误! rekey sec值无效."
var wl_Set_MIB_WLAN_WPA_GROUP_REKEY_TIME_err="设置 MIB_WLAN_WPA_GROUP_REKEY_TIME 错误!"
var wl_Set_MIB_WLAN_RS_BAND_SEL_err="设置 MIB_WLAN_RS_BAND_SEL 错误!"
var wl_Inv_802_EAP="无效的802.1x EAP类型值!"
var wl_Set_MIB_WLAN_EAP_TYPE_err="设置 MIB_WLAN_EAP_TYPE 错误!"
var wl_No_802_EAP_type="无802.1x EAP类型!"
var wl_EAP_use_ID="EAP 用户ID过长!"
var wl_Set_MIB_WLAN_EAP_USER_ID_err="设置 MIB_WLAN_EAP_USER_ID 错误!"
var wl_No_802_EAP="无802.1x EAP用户ID!"
var wl_RAD_use_nam="RADIUS 用户名过长!"
var wl_Set_MIB_WLAN_RS_USER_NAME_err="设置 MIB_WLAN_RS_USER_NAME 错误!"
var wl_No_802_RAD_name="无802.1x RADIUS用户名!"
var wl_RAD_use_pas="RADIUS用户密码过长!"
var wl_Set_MIB_WLAN_RS_USER_PASSWD_err="设置 MIB_WLAN_RS_USER_PASSWD 错误!"
var wl_No_802_RAD_password="无802.1x RADIUS用户密码!"
var wl_RAD_use_cer="RADIUS用户证书密码过长!"
var wl_Set_MIB_WLAN_RS_USER_CERT_PASSWD_err="设置 MIB_WLAN_RS_USER_CERT_PASSWD 错误!"
var wl_Cle_MIB_WLAN_RS_USER_CERT_PASSWD_err="清除 MIB_WLAN_RS_USER_CERT_PASSWD 错误!"
var wl_No_802_RAD_user="无 802.1x RADIUS 用户证书密码!"
var wl_No_802_RAD5u="无 802.1x RADIUS 5g 用户证书!请上传."
var wl_No_802_RAD5r="无 802.1x RADIUS 5g 主证书!请上传."
var wl_No_802_RAD2u="无 802.1x RADIUS 2g 用户证书!请上传."
var wl_No_802_RAD2r="无 802.1x RADIUS 2g 主证书!请上传."
var wl_Inv_802_ins="无效的 802.1x 内部隧道类型值!"
var wl_Set_MIB_WLAN_EAP_INSIDE_TYPE_err="设置 MIB_WLAN_EAP_INSIDE_TYPE 错误!"
var wl_No_802_ins="无802.1x 内部隧道类型!"
var wl_No_802_RAD_cert_pass="无802.1x RADIUS用户证书密码!"
var wl_802_ins_tun="802.1x 不支持内部隧道类型!"
var wl_802_EAP_typ="802.1x 不支持EAP类型!"
var wl_No_RS_por="无RS端口号!"
var wl_Err_Inv_val_port="错误! 无效的RS端口号."
var wl_Set_RS_por="设置 RS 端口错误!"
var wl_No_RS_IP="无 RS IP地址!"
var wl_Inv_RS_IP="无效的 RS IP地址值!"
var wl_RS_pas_len="RS 密码长度过长!"
var wl_Set_RS_pas="设置 RS 密码错误!"
var wl_Inv_RS_ret="无效的 RS 重试值!"
var wl_Set_MIB_WLAN_RS_MAXRETRY_err="设置 MIB_WLAN_RS_MAXRETRY 错误!"
var wl_Inv_RS_tim="无效的 RS 时间值!"
var wl_Set_MIB_WLAN_RS_INTERVAL_TIME_err="设置 MIB_WLAN_RS_INTERVAL_TIME 错误!"
var wl_Set_MIB_WLAN_ACCOUNT_RS_ENABLED_mib="设置 MIB_WLAN_ACCOUNT_RS_ENABLED mib 错误!"
var wl_No_acc_RS="无account RS端口号!"
var wl_Set_acc_RS="设置 account RS 端口错误!"
var wl_No_acc_RS_ip="无account RS IP地址!"
var wl_Inv_acc_RS="无效的 account RS IP地址值!"
var wl_Set_acc_RS_ip="设置 account RS IP地址错误!"
var wl_Acc_RS_pas="Account RS 密码长度过长!"
var wl_Set_acc_RS_pass="设置 account RS 密码 错误!"
var wl_Inv_acc_RS_retry="account RS重试值无效!"
var wl_Set_MIB_WLAN_ACCOUNT_RS_MAXRETRY_err="设置 MIB_WLAN_ACCOUNT_RS_MAXRETRY 错误!"
var wl_Inv_acc_RS_time="account RS时间值无效!"
var wl_Set_MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME_err="设置 MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME 错误!"
var wl_Set_MIB_WLAN_ACCOUNT_RS_UPDATE_ENABLED_mib="设置 MIB_WLAN_ACCOUNT_RS_UPDATE_ENABLED mib 错误!"
var wl_Err_Inv_val_update="错误! 无效的更新时间"
var wl_Set_MIB_WLAN_ACCOUNT_RS_UPDATE_DELAY_mib="设置 MIB_WLAN_ACCOUNT_RS_UPDATE_DELAY mib 错误!"
var wl_Err_Inv_wep="错误! 无效的 wepkeylen 值."
var wl_Set_MIB_WLAN_WEP_fai="设置 MIB_WLAN_WEP 失败!"
var wl_ssi_is_inv="ssid_idx无效!!"
var wl_Inv_wpa_val="无效的 wpaAuth 值!"
var wl_Mod_tab_ent="修改表格条目错误!"
var wl_Err_Inv_aut="错误! 无效的认证值."
var wl_Set_aut_fai="设置 设置失败!"
var wl_Err_Inv_val_frag="错误! 分段阀值值无效."
var wl_Set_fra_thr="设置分段阀值失败!"
var wl_Err_Inv_val_rts="错误! RTS阀值无效."
var wl_Set_RTS_thr="设置 RTS阀值失败!"
var wl_Err_Inv_val_beacon="错误! 无效的信标间隔."
var wl_Set_Bea_int="设置信标间隔失败!"
var wl_Err_Inv_val_ack="错误!  Ack超时值无效."
var wl_Set_Ack_tim="设置 Ack超时值失败!"
var wl_Set_rat_ada="设置 自适应速率失败!"
var wl_Set_fix_rat="设置 固定速率失败!"
var wl_Set_Tx_bas="设置 Tx 基本速率失败!"
var wl_Set_Tx_ope="设置 Tx 运行速率失败!"
var wl_Err_Inv_Pre="错误! 无效的 Preamble 值."
var wl_Set_Pre_fai="设置 前导码 失败!"
var wl_Err_Inv_hid="错误! 隐藏SSID值无效."
var wl_Set_hid_ssi="设置 隐藏SSID失败!"
var wl_Err_Inv_val_dtim="错误! DTIM阀值值无效."
var wl_Set_DTI_per="设置 DTIM 阀值失败!"
var wl_Err_Inv_IAP="错误! 无效的IAPP值."
var wl_Set_MIB_WLAN_IAPP_DISABLED_fai="设置 MIB_WLAN_IAPP_DISABLED 失败!"
var wl_Err_Inv_HS2="错误! 无效的 HS2 值."
var wl_Set_MIB_WLAN_HS2_ENABLED_fai="设置 MIB_WLAN_HS2_ENABLED 失败!"
var wl_Err_Inv_11g="错误! 无效的 11g Protection 值."
var wl_Set_MIB_WLAN_PROTECTION_DISABLED_fai="设置 MIB_WLAN_PROTECTION_DISABLED 失败!"
var wl_Err_Inv_WMM="错误! 无效的 WMM 值."
var wl_Set_MIB_WLAN_WMM_ENABLED_fai="设置 MIB_WLAN_WMM_ENABLED 失败!"
var wl_Err_Inv_tur="错误! 无效的 turbo mode 值."
var wl_Set_MIB_WLAN_TURBO_MODE_fai="设置 MIB_WLAN_TURBO_MODE 失败!"
var wl_Err_Inv_RF="错误! 无效的 RF 输出功率值."
var wl_Set_MIB_WLAN_RFPOWER_SCALE_fai="设置 MIB_WLAN_RFPOWER_SCALE 失败!"
var wl_Err_Inv_Cha="错误! 无效的 信道绑定."
var wl_Set_MIB_WLAN_CHANNEL_BONDING_fai="设置 MIB_WLAN_CHANNEL_BONDING 失败!"
var wl_Err_Inv_Con="错误! 无效的 控制边带."
var wl_Set_MIB_WLAN_CONTROL_SIDEBAND_fai="设置 MIB_WLAN_CONTROL_SIDEBAND 失败!"
var wl_Set_MIB_WLAN_AGGREGATION_fai="设置 MIB_WLAN_AGGREGATION 失败!"
var wl_Err_Inv_sho="错误! 无效的 short GI."
var wl_Set_MIB_WLAN_SHORT_GI_fai="设置 MIB_WLAN_SHORT_GI 失败!"
var wl_Sit_req_fai="站点扫描请求失败!"
var wl_Che_enc_err="校验加密错误!"
var wl_Enc_typ_mis="加密方式不匹配!"
var wl_Joi_req_fai="加入请求失败!"
var wl_conreq_tim_="连接请求超时!"
var wl_Get_Joi_res="获取加入结果失败!"
var wl_con_tim_="连接超时!"
var wl_enc_val_not="加密值无效!"
var wl_Inv_wep_key="无效的 wep 密钥格式!"
var wl_Inv_wep_key_len="无效的 wep 密钥长度!"
var wl_Err_no_act="错误, 未定义行为!"
var wl_Err_inv_act="错误, 无效的行为请求!"
var wl_Inv_mod_val="无效的模式值!"
var wl_The_que_sel="Q&A模式的问题选择或答案与内置值不匹配!"
var wl_Can_not_set_key="不能设置广播 key policy!"
var wl_Can_not_set_tim="不能设置广播 时间!"
var wl_Can_not_set_pac="不能设置广播包!"
var wl_Can_not_set_pol="不能设置单播 key policy!"
var wl_Can_not_set_utime="不能设置单播时间!"
var wl_Can_not_set_upacket="不能设置单播包!"
var wl_Set_Sea_MIB="设置 Search MIB Index 错误!"
var wl_Not_a_Cer="不是证书类型!"
var wl_Can_not_ope="不能够打开临时证书!"
var wl_Unk_Cer_Fil="未知的证书类型!"
var wl_Cer_Ins_Suc="证书安装成功!"
var wl_Set_Sea_Ind="设置搜索索引错误!"
var wl_Set_Sea_Inf="设置搜索信息错误!"
var set_MIB_WLAN_RS_BAND_SEL_err="设置 MIB_WLAN_RS_BAND_SEL 错误!"
var wl_Wro_rsB_="错误 rsBandSel !"
var wl_Del_all_802_5="删除所有5GHz 802.1x 证书成功!"
var wl_Del_all_802_2="删除所有2.4GHz 802.1x 证书成功!"
var wl_No_802_cer="上传证书未包含802.1x证书!"
var wl_No_802_pri="上传文件未包含802.1x私有密钥!"
var wl_802_use_cer="802.1x 用户证书和用户密码上传成功!"
var wl_No_802_cert_inclued="[2] 上传文件不包括No 802.1x 证书!"
var wl_not_ssid_type="错误! 不支持此SSID类型."
var wl_802_roo_cer="802.1x根证书上传成功!"
var wl_err_han_="错误处理"
var wl_Set_CA_sel="设置 CA select 错误!"
var wl_Set_Use_sel="设置 User select 错误!"
var wl_Err__Upl="错误 ! 上传文件长度为 0 !"
var wl_Aut_sca_run="自动扫描正在运行!!请等待..."
var wl_sca_req_tim="扫描请求超时!"
var wl_sca_tim_="扫描超时!"
var wl_no_nas="错误! 未设置 NAS identifier."
var wl_err_no_r0r1_key="错误! 未设置 R0KH/R1KH key."
var wl_err_no_op_class="错误! 未设置 Op Class."
var wl_no_channel="错误! 未设置信道."
var wl_get_mbssid_tbl_error="错误! 获取 MIB_MBSSIB_TBL 错误."
var wl_entry_exists="条目已存在!"
var wl_err_table_full="错误! 表已满."
var wl_err_add_chain= "错误! 添加链表记录."
var wl_del_chain_record="删除链表记录错误!"
var wl_no_item_selected="没有选择要删除的条目!"
var wl_disabled="禁用"
var wl_enabled="启用"
var wl_error_handle="错误处理"
var wl_open_file_error="打开文件错误"
/***********	andlink.htm ************/
var andlink_header='andlink'
var andlink_explain='启用andlink，家庭终端将自配置接入智能家庭网关。'
var andlink_disable='禁用'
var andlink_enable='启用'
var andlink_mode="模式"
var andlink_mode_bridge="桥"
var andlink_mode_repeater="中继"
var andlink_local_ip_addr="IP地址"
var andlink_version="版本"
var andlink_search_sync_type="同步类型"
var andlink_search_sync="同步"
var andlink_search_async="异步"
var andlink_heartbeat_port="心跳端口"
var andlink_connect_gw_timeout="网关连接超时"
var andlink_get_ip_timeout="获取ip超时"
var andlink_recovery="恢复初始状态"
var andlink_recovery_enable="是"
var andlink_recovery_disable="否"
var andlink_auto_dhcp='auto dhcp'
var andlink_auto_dhcp_enable='开启'
var andlink_auto_dhcp_disable='关闭'
var andlink_connect_gw_range="网关连接超时值无效,值应介于0-255之间."
var andlink_get_ip_range="获取IP超时值无效,值应介于0-255之间."


var andlink_reset = ' 重置 ';
var andlink_save = '保存';
var andlink_save_apply = '保存 & 生效';

/***********	CONFIG_CUTE_MAHJONG ************/
var opmode_radio_upgradekit='Upgrade Kit:';
var opmode_radio_upgradekit_explain='In this mode all ethernet ports and wireless interfaces are bridged together and the NAT function is disabled.';
var opmode_radio_gw_explain_cmj='In this mode, the device connects to internet via an ADSL/Cable Modem. NAT is enabled and PCs on LAN ports share the same IP to the ISP via the WAN port. The connection type is DHCP client.';
var opmode_radio_wisp_explain_cmj='In this mode, all ethernet ports are bridged together and the wireless client will connect to the ISP’s Access Point. NAT is enabled and PCs on Ethernet ports share the same IP to the ISP via the wireless LAN. You can connect to the ISP’s AP on the Site-Survey page. The connection type is DHCP client.';
var opmode_radio_auto_explain_cmj='In this mode, the WAN detection function will automatically boot in Gateway or WISP mode.';

var wizard_cmj_content1 = 'Set Operating Mode';
var wizard_cmj_content2 = 'Set Wireless Network Name';
var wizard_cmj_content2_explain = 'Enter the Wireless Network Name of the AP.';
var wizard_cmj_content2_explain2 = 'Wireless Network Name (SSID):';
var wizard_cmj_content3 = 'Select the Wireless Security Mode';

var wizard_sitesurvey_content = 'Wireless Site Survey';
var wizard_sitesurvey_content_explain = 'This page provides a tool to scan for wireless networks. If an Access Point or IBSS is found, you can choose to connect to it manually when client mode is enabled.';

var switch_ui_mode2adv  = '切换至高级模式';
var switch_ui_mode2easy = '切换至精简模式';
/***********	OK_MSG&ERR_MSG ************/
var okmsg_explain = "<body><blockquote><h4>改变配置成功!</h4>你的改变已经保存.路由器必须重启以使改变生效.<br> 你可以立刻重启, 你也可以继续修改稍后再重启.\n";
var okmsg_reboot_now = '立刻重启';
var okmsg_reboot_later = '稍后重启';

var okmsg_btn = '  确定  ';

var okmsg_fw_saveconf = '重新载入配置成功!<br><br>路由器正在重启.<br>在这段时间内不要关闭或重启路由器.<br>';
var okmsg_fw_opmode = '设置操作模式成功!';
var okmsg_fw_passwd = '密码修改成功!<br><br>在这段时间内不要重启或关闭路由器.';

/*********** common msg ************/
var ip_should_in_current_subnet = '无效的IP地址,IP地址必须在当前子网的范围内.';
var ip_should_be_different_from_lanip = '无效的IP地址,IP地址不能与lan ip相同.';

function dw(str)
{
	document.write(str);
}
