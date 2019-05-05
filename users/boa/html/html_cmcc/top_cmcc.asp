<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
<TITLE>家庭网关</TITLE>
<META http-equiv=pragma content=no-cache>
<META http-equiv=cache-control content="no-cache, must-revalidate">
<META http-equiv=content-type content="text/html; charset=utf-8">
<META http-equiv=content-script-type content=text/javascript>
<!--系统公共css-->
<STYLE type=text/css>
@import url(/style/default.css);
</STYLE>
<!--系统公共脚本-->
<SCRIPT language="javascript" src="common.js"></SCRIPT>
<SCRIPT language="javascript" type="text/javascript">
//var timeout = 300000;	/* 5 min */
var timeout = 900000;	/* 15 min */
var timeoutTimer;

timeoutTimer = setTimeout(timeoutFunc, timeout);

function clearAuthData()
{
	
  if (document.all) 
  {
    // Internet Explorer: 'ClearAuthenticationCache' is only available in IE
    document.execCommand('ClearAuthenticationCache');

	} 
	else 
	{
    var xmlhttp;
    if (window.XMLHttpRequest) 
   	{
        xmlhttp = new XMLHttpRequest();
    } 
    else if (window.ActiveXObject) 
    {
        try {
            xmlhttp = new ActiveXObject('Msxml2.XMLHTTP');
        } catch (ex) {
            try {
                xmlhttp = new ActiveXObject('Microsoft.XMLHTTP');
            } catch (ex) {}
        }
    }
    if (xmlhttp.readyState < 4) 
    {
        xmlhttp.abort();
    }
    // Firefox/Mozilla: use anonymous "login" to trigger a "logout"
    xmlhttp.open('GET', '/?sling:authRequestLogin=1', false, 'anonymous', 'null');
    xmlhttp.send('');
}
		document.logout.submit();
		return true;

}
function timeoutFunc()
{
	clearAuthData();
	document.forms[0].submit();
}

function resetTimeoutFunc()
{
	clearTimeout(timeoutTimer);
	timeoutTimer = setTimeout(timeoutFunc, timeout);
}
/********************************************************************
**          menu class
********************************************************************/
function menu(name)
{
	this.name = name;
	this.names = new Array();
	this.objs = new Array();
	
	this.destroy = function(){delete map;map = null;}
	this.add = function(obj, name){var i = this.names.length; if(name){this.names[i] = name;}else{this.names[i] = obj.name;} this.objs[i] = obj;}
	
	return this;
}

var mnroot = new menu("root");
var mncata = null;
var mnpage = null;
var wlan_num =  <% getIndex("show_wlan_num"); %> ;
/* cmcc_mode
  0: AP_PACKAGE 
	1: GATEWAY
	2: BRIDGE
*/
var cmcc_mode =  <% getIndex("cmcc_mode"); %> ;
mncata = new menu("状态");mnpage = new menu("设备信息");
mnpage.add("status.htm","设备基本信息");
mnpage.add("stats.htm","统计信息");
mncata.add(mnpage);
/*
mnpage = new menu("网络侧信息");
mnpage.add("status_net_connet_info.asp","IPv4连接信息");
mnpage.add("status_net_connet_info_ipv6.asp","IPv6连接信息");
//mnpage.add("status_gpon.asp","PON链路连接信息");
mncata.add(mnpage);
mnpage = new menu("用户侧信息");
mnpage.add("status_wlan_info_11n_24g_cmcc.asp","WLAN2.4G接口信息");
mnpage.add("status_wlan_info_11n_5g_cmcc.asp","WLAN5G接口信息");
mnpage.add("status_ethernet_info_cmcc.asp","LAN接口信息");
//mnpage.add("status_usb_info.asp","USB接口信息");
mncata.add(mnpage);
mnpage = new menu("宽带语音信息");
mnpage.add("cmcc_status_voip_info.asp","宽带语音信息");
mncata.add(mnpage);

mnpage = new menu("远程管理状态");
mnpage.add("status_tr069_info_admin_cmcc.asp","交互建立");
mnpage.add("status_tr069_config_admin_cmcc.asp","业务配置下发状态");
mncata.add(mnpage);*/
mnroot.add(mncata);
mncata = new menu("网  络");
if(cmcc_mode == 1 || cmcc_mode == 2){
  mnpage = new menu("网络模式");
  mnpage.add("opmode.htm","网络模式设置");
  mncata.add(mnpage);
}
mnpage = new menu("Internet设置");
if(cmcc_mode == 1)
  mnpage.add("net_fastconf.asp","快速配置向导");
if(cmcc_mode == 1)
  mnpage.add("tcpipwan.htm","WAN侧连接");
mnpage.add("tcpiplan.htm","LAN侧连接");
mncata.add(mnpage);
if(<%getIndex("ipv6");%>){
	mnpage = new menu("IPV6设置");
	mnpage.add("ipv6_wan.htm","IPv6 Wan侧设置");
	mnpage.add("dhcp6s.htm","IPv6 Lan侧设置");
	mnpage.add("radvd.htm","Radvd设置");
	mnpage.add("tunnel6.htm","IPv6隧道");
	if(<% getIndex("is_mape"); %>)
		mnpage.add("map_e.htm","MAP-E");
	mncata.add(mnpage);
}
/*mnpage = new menu("LAN侧地址配置");
mnpage.add("net_dhcpd_cmcc.asp","IPv4配置");
mnpage.add("net_ipv6_cmcc.asp","IPv6 配置");
mncata.add(mnpage);*/
if(cmcc_mode == 1){
  mnpage = new menu("QoS");
  if(<%getIndex("ipv6");%>)
  	mnpage.add("ip6_qos.htm","QOS控制");
  else
  	mnpage.add("ip_qos.htm","QOS控制");
  mncata.add(mnpage);
}
if(wlan_num>1){
mnpage = new menu("WLAN5G配置");
mnpage.add("boafrm/formWlanRedirect?redirect-url=wlbasic.htm&wlan_id=0","WLAN基本配置");
mnpage.add("wladvanced.htm","高级配置");
mnpage.add("wlsecurity.htm","安全配置");
mnpage.add("wlactrl.htm","连接控制");
mnpage.add("wlwds.htm","WDS配置");
mnpage.add("wlsurvey.htm","网络扫描");
mnpage.add("wlwps.htm","WPS配置");
mnpage.add("wlsch.htm","WIFI连接规则");
mncata.add(mnpage);
}
if(wlan_num>0){
mnpage = new menu("WLAN2.4G配置");
mnpage.add("boafrm/formWlanRedirect?redirect-url=wlbasic.htm&wlan_id=1","WLAN基本配置");
mnpage.add("wladvanced.htm","高级配置");
mnpage.add("wlsecurity.htm","安全配置");
mnpage.add("wlactrl.htm","连接控制");
mnpage.add("wlwds.htm","WDS配置");
mnpage.add("wlsurvey.htm","网络扫描");
mnpage.add("wlwps.htm","WPS配置");
mnpage.add("wlsch.htm","WIFI连接规则");
mncata.add(mnpage);
}
//mnpage = new menu("远程管理");
//mnpage.add("tr069config.htm","省级数字家庭管理平台服务器");
//mnpage.add("usereg_inside_loid_cmcc.asp","LOID认证");
//mnpage.add("usereg_inside_menu_cmcc.asp","PASSWORD认证");
//mncata.add(mnpage);
mnpage = new menu("时间管理");
mnpage.add("ntp.htm","时间管理");
mncata.add(mnpage);
if(cmcc_mode == 1){
  mnpage = new menu("路由配置");
  mnpage.add("route.htm","静态路由");
  mncata.add(mnpage);
}
mnroot.add(mncata);

if(cmcc_mode == 1){
  mncata = new menu("安  全");
  mnpage = new menu("广域网访问设置");
  mnpage.add("urlfilter.htm","广域网访问设置");
  mncata.add(mnpage);
  mnpage = new menu("防火墙");
  //mnpage.add("secu_firewall_level_cmcc.asp","安全级");
  mnpage.add("dos.htm","攻击保护设置");
  mncata.add(mnpage);
  mnpage = new menu("MAC过滤");
  mnpage.add("macfilter.htm","MAC过滤");
  mncata.add(mnpage);
  mnpage = new menu("IP过滤");
  if(<%getIndex("ipv6");%>)
  	mnpage.add("ip6filter.htm","IP过滤");
  else
  	mnpage.add("ipfilter.htm","IP过滤");
  mncata.add(mnpage);
  mnpage = new menu("端口过滤");
  if(<%getIndex("ipv6");%>)
  	mnpage.add("portfilter6.htm","端口过滤");
  else
  	mnpage.add("portfilter.htm","端口过滤");
  mncata.add(mnpage);
  /*
  if(<%getIndex("vlan_binding_support");%>)
  {
  	mnpage = new menu("VLAN绑定");
  	mnpage.add("vlan_binding.htm","VLAN绑定");
  	mncata.add(mnpage);
  }
  */
  if(<%getIndex("linux_vlan_support");%>)
  {
  	mnpage = new menu("VLAN绑定");
  	mnpage.add("8021q_vlan.htm","802.1Q VLAN");
  	mncata.add(mnpage);
  }
  mnroot.add(mncata);
}
if(cmcc_mode == 1){
  mncata = new menu("应  用");
  mnpage = new menu("DDNS配置");
  mnpage.add("ddns.htm","DDNS配置");
  mncata.add(mnpage);
  mnpage = new menu("高级NAT配置");
  //mnpage.add("algonoff.asp","ALG配置");
  mnpage.add("dmz.htm","DMZ配置");
  mnpage.add("portfw.htm","虚拟主机配置");
  mncata.add(mnpage);
  /*
  mnpage = new menu("UPNP配置");
  mnpage.add("app_upnp_cmcc.asp","UPNP配置");
  mncata.add(mnpage);
  */
  //mnpage = new menu("宽带电话设置");
  //mnpage.add("cmcc_app_voip.asp","宽带电话设置");
  //mncata.add(mnpage);
  //mnpage = new menu("IGMP设置");
  //mnpage.add("snooping_proxy_cmcc.asp","IGMP设置");
  //mncata.add(mnpage);
  mnroot.add(mncata);
}
mncata = new menu("管  理");
mnpage = new menu("用户管理");
mnpage.add("password.htm","用户管理");
mncata.add(mnpage);
mnpage = new menu("设备管理");
//mnpage.add("cluster_mgmt.asp","集群管理");
mnpage.add("mgm_dev_reboot.asp","设备重启");
mnpage.add("saveconf.htm","恢复配置");
mnpage.add("upload.htm","固件更新");
mncata.add(mnpage);
mnpage = new menu("日志文件管理");
mnpage.add("syslog.htm","日志管理");
//mnpage.add("mgm_log_view_cmcc.asp","日志查看");
mncata.add(mnpage);
if(<% getIndex("isDisplayTR069"); %>){
	mnpage = new menu("远程管理");
	mnpage.add("tr069config.htm","TR-069配置");
	mncata.add(mnpage);
}
mnroot.add(mncata);
mncata = new menu("诊断");
mnpage = new menu("网络诊断");
mnpage.add("diag_ping_admin.asp","PING测试");
mnpage.add("diag_tracert_admin.asp","Tracert测试");
//mnpage.add("diagnose_tr069_admin_cmcc.asp","Inform手动上报");
mncata.add(mnpage);
/*
mnpage = new menu("VoIP诊断");
mnpage.add("auto_call_voip.asp","VoIP诊断");
mncata.add(mnpage);
*/
mnroot.add(mncata);
mncata = new menu("帮  助");
mnpage = new menu("状态");
mnpage.add("/help_cmcc/help_status_device.html","设备信息");
mnpage.add("/help_cmcc/help_status_net.asp","网络侧信息");
mnpage.add("/help_cmcc/help_status_user.html","用户侧信息");
//mnpage.add("/help/help_status_voip.html","宽带语音信息");
mnpage.add("/help_cmcc/help_status_tr069.html","远程管理状态");
mncata.add(mnpage);
mnpage = new menu("网络");
mnpage.add("/help_cmcc/help_net_pon.html","宽带设置");
mnpage.add("/help_cmcc/help_net_vlan_binding.html","绑定配置");
mnpage.add("/help_cmcc/help_net_lan.html","LAN侧地址配置");
mnpage.add("/help_cmcc/help_net_wlan.asp","WLAN网络配置");
mnpage.add("/help_cmcc/help_net_remote.asp","远程管理");
mnpage.add("/help_cmcc/help_net_qos.html","QoS");
mnpage.add("/help_cmcc/help_net_time.html","时间管理");
mnpage.add("/help_cmcc/help_net_route.html","路由配置");
mncata.add(mnpage);
mnpage = new menu("安全");
mnpage.add("/help_cmcc/help_security_wanaccess.html","广域网访问设置");
mnpage.add("/help_cmcc/help_security_firewall.html","防火墙");
mnpage.add("/help_cmcc/help_security_macfilter.html","MAC过滤");
mnpage.add("/help_cmcc/help_security_portfilter.html","端口过滤");
mncata.add(mnpage);
mnpage = new menu("应用");
mnpage.add("/help_cmcc/help_apply_ddns.html","DDNS配置");
mnpage.add("/help_cmcc/help_apply_nat.html","高级NAT配置");
mnpage.add("/help_cmcc/help_apply_upnp.html","UPNP配置");
//mnpage.add("/help/help_apply_voip.html","宽带电话设置");
mnpage.add("/help_cmcc/help_apply_igmp.html","IGMP设置");
mncata.add(mnpage);
mnpage = new menu("管理");
mnpage.add("/help_cmcc/help_manage_user.html","用户管理");
mnpage.add("/help_cmcc/help_manage_device.asp","设备管理");
mnpage.add("/help_cmcc/help_manage_logfile.html","日志文件管理");
mnpage.add("/help_cmcc/help_manage_tr069.html","远程管理");
mncata.add(mnpage);
mnpage = new menu("诊断");
mnpage.add("/help_cmcc/help_diag_net.html","网络诊断");
//mnpage.add("/help/help_diag_voip.html","VOIP诊断");
mncata.add(mnpage);
mnroot.add(mncata);



/********************************************************************
**          on document load
********************************************************************/
var NavImage = new Array();
NavImage[0]=new Array("nav_condition_n.png");
NavImage[1]=new Array("nav_network_n.png");
NavImage[2]=new Array("nav_security_n.png");
NavImage[3]=new Array("nav_yingyong_n.png");
NavImage[4]=new Array("nav_administration_n.png");
NavImage[5]=new Array("nav_diagnosis_n.png");
NavImage[6]=new Array("nav_help_n.png");
var NavImageClick = new Array();
NavImageClick[0]=new Array("nav_condition_h.png");
NavImageClick[1]=new Array("nav_network_h.png");
NavImageClick[2]=new Array("nav_security_h.png");
NavImageClick[3]=new Array("nav_yingyong_h.png");
NavImageClick[4]=new Array("nav_administration_h.png");
NavImageClick[5]=new Array("nav_diagnosis_h.png");
NavImageClick[6]=new Array("nav_help_h.png");

function on_init()
{
	var fst = null;
	
	if(!topmenu) topmenu = document.getElementById("topmenu");
	if(!submenu) submenu = document.getElementById("submenu");
	
	if(topmenu.cells){while(topmenu.cells.length > 0) topmenu.deleteCell(0);}
	
	for(var i = 0; i < mnroot.names.length; i++)
	{
		var cell = topmenu.insertCell(i);
        var txt = "<a href=\"#\" onClick=\"on_catalog(" + i + ");\"><div class=\"menusize\"><img  id=\"catalogimg"+i+"\"src=\"image/"+NavImage[i]+"\"><br>"; 
		//txt += "<a href=\"#\" onClick=\"on_catolog(" + i + ");\">";
		txt += "<font style=\"font-size:14px;font-weight:bold;\" id=\"catalogfont"+i+"\">" + mnroot.names[i] + "</font></div></a>";
		//cell.bgColor = "#EF8218";
		cell.width = "95";
		cell.align = "center";
		//cell.style = "line-height: 25px;";
		cell.innerHTML = txt;
		cell.mnobj = mnroot.objs[i];
		if(fst == null)fst = i;
	}
	topmenu.sel = 0;
	//topmenu.cells[0].bgColor = "#427594";
	document.getElementById("catalogimg0").src="image/"+NavImageClick[0];
	//document.getElementById("catalogfont0").style="color:#fff45c;font-size:14px;font-weight:bold;";
		document.getElementById("catalogfont0").style.color="fff45c"
		document.getElementById("catalogfont0").style.fontSize="14px"
		document.getElementById("catalogfont0").style.fontWeight="bold"
	
	
	//menuname.innerHTML = mnroot.names[0];
	on_catalog(fst);
}

/********************************************************************
**          on_catalog changed
********************************************************************/
function on_catalog(index)
{
	var fst = null;
	
	if(!topmenu.cells || index >= topmenu.cells.length)return;
	
	if(topmenu.sel != index)
	{
		//topmenu.cells[topmenu.sel].bgColor = "#EF8218";
		document.getElementById("catalogimg"+topmenu.sel).src="image/"+NavImage[topmenu.sel];
		//document.getElementById("catalogfont"+topmenu.sel).style="font-size:14px;font-weight:bold;";
		document.getElementById("catalogfont"+topmenu.sel).style.fontSize="14px";  
		document.getElementById("catalogfont"+topmenu.sel).style.fontWeight="bold";  
		document.getElementById("catalogfont"+topmenu.sel).style.color="#ffffff";
		//topmenu.cells[index].bgColor = "#427594";
		document.getElementById("catalogimg"+index).src="image/"+NavImageClick[index];
	//	document.getElementById("catalogfont"+index).style="color:#fff45c;font-size:14px;font-weight:bold;";
		document.getElementById("catalogfont"+index).style.color="#fff45c";
		document.getElementById("catalogfont"+index).style.fontSize="14px";
		document.getElementById("catalogfont"+index).style.fontWeight="bold";
		//document.getElementById("menu"+index).color="#fff45c";
		topmenu.sel = index;
		//menuname.innerHTML = mnroot.names[index];
	}
	
	var mnobj = topmenu.cells[index].mnobj;
	
	if(submenu.cells){while(submenu.cells.length > 1) submenu.deleteCell(1);}

	for(var i = 0; i < mnobj.names.length; i++)
	{
		var cell = submenu.insertCell(i * 2 + 1);
		var index = i * 2 + 2;
		cell = submenu.insertCell(index);
		var txt ="<a href=\"#\" onClick=\"on_menu(" + index + ");\">";
        txt += "<span class=\"menu_space\" id=\"menufont"+index+"\">" + mnobj.names[i] + "</span></a>";
		//cell.width = "75px";
		cell.innerHTML = txt;
		cell.nowrap = true;
		cell.name = mnobj.names[i];
		cell.mnobj = mnobj.objs[i];
		if(fst == null)fst = index;
	}
	submenu.sel=fst;
	document.getElementById("menufont"+fst).style.color="#fff45c";
	document.getElementById("menufont"+fst).style.fontSize="12px";
	on_menu(fst);
}

/********************************************************************
**          on menu fire
********************************************************************/
function on_menu(index)
{
	if(!submenu.cells || index >= submenu.cells.length)return;
	
	if(submenu.sel != index)
	{
		//document.getElementById("menufont"+submenu.sel).style="color:#fff45c;font-size:12px;";
		//document.getElementById("menufont"+index).style="color:#fff45c;font-size:12px;";
		document.getElementById("menufont"+submenu.sel).style.color="#ffffff";
		document.getElementById("menufont"+submenu.sel).fontSize="12px";
		document.getElementById("menufont"+index).style.color="#fff45c";
		document.getElementById("menufont"+index).style.fontSize="12px";
		submenu.sel = index;
	}
	
	tbobj = submenu.cells[index];
	var mnobj = tbobj.mnobj;
	//var lstmenu = leftFrame.lstmenu;
	if(!lstmenu) lstmenu = leftFrame.document.getElementById("lstmenu");
	if(!lstmenu)return;
	if(lstmenu.rows){while(lstmenu.rows.length > 0) lstmenu.deleteRow(0);}
	
	for(var i = 0; i < mnobj.names.length; i++)
	{
		var row = lstmenu.insertRow(i);
		
		row.nowrap = true;
		row.vAlign = "top";
		
		var cell = row.insertCell(0);
		
		cell.width = "100%";
		cell.align = "center";
		if(i == 0){		
			cell.innerHTML = "<br><p>&nbsp;&nbsp;<a id=\"thirdmenufont"+i+"\" onClick=\"on_thirdmenu(" + i + ")\"; style=\"color:#fff45c\" href=\"" + mnobj.objs[i] + "\", target=\"contentIframe\">" + mnobj.names[i] + "</a></p>";
		}else{
			cell.innerHTML = "<br><p>&nbsp;&nbsp;<a id=\"thirdmenufont"+i+"\" onClick=\"on_thirdmenu(" + i + ")\"; href=\"" + mnobj.objs[i] + "\", target=\"contentIframe\">" + mnobj.names[i] + "</a></p>";
		}
		cell.nowrap = true;
		cell.name = mnobj.names[i];
		cell.mnobj = mnobj.objs[i];
	}
	contentIframe.location.href=mnobj.objs[0];
}

function on_thirdmenu(index){
	tbobj = submenu.cells[submenu.sel ];
	var mnobj = tbobj.mnobj;
	for(var i = 0; i < mnobj.names.length; i++){
		document.getElementById("thirdmenufont"+i).style.color="#ffffff";
	}
	//document.getElementById("thirdmenufont"+index).style="color:#fff45c";
	document.getElementById("thirdmenufont"+index).style.color="#fff45c";

}
function contenFramesize()
{
	getElById('contentIframe').style.height=600; 
	var mainbody = contentIframe.document.body.scrollHeight;
	var trmainbody = getElById('trmain').clientHeight;
	var mainbodyoffset = getElById('contentIframe').offsetHeight;
	var end = mainbody;
	if (end < (trmainbody-31))
		end = trmainbody-31;
	getElById('contentIframe').style.height=end;	//must be id
}

function getElById(idVal) {
	if (document.getElementById != null)
	return document.getElementById(idVal)
	if (document.all != null)
	return document.all[idVal]	
	alert("Problem getting element by id")
	return null;
}		

</SCRIPT>
<style>
.toplogo{
	background:#2bbdd4;
	width:180px;
	height:50px;
	padding-left: 80px;
}
.type_cmcc{
	background:#2bbdd4;
	width:605px;
	color:#fff;
	font-size: 15px;
}
.welcomeLink{
	background:#2bbdd4;
	color: #fff;
    font-size: 14px;
    font-weight: bold;
	display: inline-block;
	padding-right: 10px;
	padding-top: 15px;
}
.welcomeLink input{
	background:0 none;
	border: 0 none;
	color: #fff;
	font-weight: bold;
}
#topmenu span{
	color:#fff;
}
#topmenu  a{
	color:#fff;
	TEXT-DECORATION: none;
	font-family: "微软雅黑";
}
#lstmenu a{
	color: #fff;
	TEXT-DECORATION: none;
	font-weight: bold;
	font-family: "微软雅黑";
}
#submenu a{
	color:#fff;
	TEXT-DECORATION: none;
	font-family: "微软雅黑";
	cursor:pointer;
}
.menu_space {
    display: inline-block;
    padding: 0 8px;
}
#contentIframe {
    background: #f8f8f8;
}
.menusize {
    color: #ffffff;
    font-size: 14px;
    font-weight: bold;
    height: 70px;
    line-height: 25px;
    text-align: center;
    text-decoration: none;
    width: 70px;
	cursor:pointer;
}
p{
	text-align: center;
}
</style>
</HEAD>

<!-------------------------------------------------------------------------------------->
<!--主页代码-->
<body topmargin="0" leftmargin="0" marginwidth="0" marginheight="0" alink="#000000" link="#000000" vlink="#000000" onLoad="on_init();" align=center;>
<form action=/boafrm/formLogout method=POST name="top" target="_top">
	<table border="0" width="980" cellspacing="0" cellpadding="0" align="center" height="50">
		<tr bgcolor="#2bbdd4">
		<!--	<td class="toplogo"> 
				<img class="logoimg" src="image/mobile2.png">
			</td>
			<td class="type_cmcc">
			  型号：  CMCC
			</td>-->
			<td class="type_cmcc">
			  家庭网关
			</td>
			<td class="welcomeLink" align="right" valign="center">
			欢迎您！&nbsp;<input type="submit"      value="退出" onclick="return clearAuthData()">
			</td>
		</tr>
	</table>
	
	<table border="0" cellpadding="0" cellspacing="0" width="980" align="center"> 
		<tr nowrap bgcolor="#2bbdd4"> 
			<td width="150" rowspan="3" bgcolor="#2bbdd4" id="menuname">&nbsp;
			<td height="32"  width="830" style="font-size:9pt" align="right"></td>
		</tr>
		<tr> 
			<td height="70" bgcolor="#2bbdd4"> 
				<table border="0" cellpadding="0" cellspacing="0" width="665" height="70" bgcolor="#2bbdd4"> 	
					<tr id="topmenu" nowrap style="color:#fff;">
					  <td align="center" width="70">　</td>
					</tr>
				</table>
			</td>
		</tr>
	  <tr> 
		<td height="34" bgcolor="#303841"> 
		<table border="0" cellpadding="0" cellspacing="0" height="30">
			<tr id="submenu" style="font-size:9pt; color:#C0C0C0" nowrap> 
			  <td>　</td>
			</tr>
		</table>
		</td>
	  </tr>
	</table>


<div name="leftFrame" class="leftframe" style="margin:0px auto; width:980px; text-align: center;">


<table border="0" cellpadding="0" cellspacing="0">
  <tr valign="top" id='trmain'>
    <td bgcolor="#363e47" valign="top">
		<table border="0" cellpadding="0" cellspacing="0" width="150" id="lstmenu">
		<tr><td></td></tr> 
		</table>
	</td>
	<td width="830">
	<iframe id="contentIframe" name="contentIframe" align="middle" src="status_device_basic_info.asp" frameborder="0" width="830"  onload="contenFramesize();"></iframe>
	</td>
  </tr>
</table>
</div>

		
	<table cellSpacing=0 cellPadding=0 width=980 border=0 align="center">
		<tr>
			<td	bgcolor="#f8f8f8" width="150" height="50" align="right"></td>
			<td	bgcolor="#f8f8f8" width="680" height="50" align="center"><label size="12px" color="#333333"></label></td>
			<td	bgcolor="#f8f8f8" width="150" height="50" align="right"></td>
		</tr>
	</table>

	

</form>
</body>

</html>
