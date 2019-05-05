<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>广域网络接口设置 </title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<script type="text/javascript" src="js/util_gw.js"> </script>
<SCRIPT language=Javascript src="js/language_sc.js"></SCRIPT>
<link href="css/style.css" rel="stylesheet" type="text/css">
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<style type="text/css">
.ipv6Input{
	font-size: 12px;margin-bottom: 4px;
	height: 26px;
    border: 1px #d2d2d2 solid;
    width: 40px;
    font-family: "微软雅黑";
    color: #666;
    padding: 0 8px;
    text-align: center;
}
.cfg td label {
    line-height: 36px;
}
</style>
<script>
var initialDnsMode, pppConnectStatus=0;
var wlan_num =  <% getIndex("show_wlan_num"); %> ;
var ipv6WanEnable = <%getIPv6WanInfo("ipv6WanEnabled")%>;

function resetClicked()
{
	document.tcpip.reset;

	location=location;
}

function abortClicked()
{
	document.tcpip.ChangeNotSave.value = "yes";	
}
function disableDNSinput()
{
	//disable dns1
   disableTextField(document.tcpip.wan_dns1_0);
   disableTextField(document.tcpip.wan_dns1_1);
   disableTextField(document.tcpip.wan_dns1_2);
   disableTextField(document.tcpip.wan_dns1_3);
   disableTextField(document.tcpip.wan_dns1_4);
   disableTextField(document.tcpip.wan_dns1_5);
   disableTextField(document.tcpip.wan_dns1_6);
   disableTextField(document.tcpip.wan_dns1_7);
   disableTextField(document.tcpip.prefix_len_dns1);	
}

function enableDNSinput()
{
   	//enable dns1
   enableTextField(document.tcpip.wan_dns1_0);
   enableTextField(document.tcpip.wan_dns1_1);
   enableTextField(document.tcpip.wan_dns1_2);
   enableTextField(document.tcpip.wan_dns1_3);
   enableTextField(document.tcpip.wan_dns1_4);
   enableTextField(document.tcpip.wan_dns1_5);
   enableTextField(document.tcpip.wan_dns1_6);
   enableTextField(document.tcpip.wan_dns1_7);
   enableTextField(document.tcpip.prefix_len_dns1);
	
}

function autoDNSclicked()
{
  disableDNSinput();
  document.tcpip.dnsType.value=1;
}

function manualDNSclicked()
{
  enableDNSinput();
  document.tcpip.dnsType.value=0;
}
function statelessDhcpClicked()
{
	document.tcpip.dhcpType.value=1;
	document.tcpip.enable_dhcpv6pd.disabled=true;
	document.tcpip.enable_dhcpv6RapidCommit.disabled=true;
	document.getElementById('pd_div').style.display = 'none';
	
	manualDNSclicked();
}
function statsfulDhcpClicked()
{
	document.tcpip.dhcpType.value=0;
	document.tcpip.enable_dhcpv6pd.disabled=false;
	document.tcpip.enable_dhcpv6RapidCommit.disabled=false;
	document.getElementById('pd_div').style.display = '';
	
	if(document.tcpip.elements["dnsMode"][0].checked == true)
	{//auto
		autoDNSclicked();
	}else
		manualDNSclicked();
}
function pppConnectClick(connect)
{
  if (document.tcpip.pppConnectType.selectedIndex == 2 && pppConnectStatus==connect) {
      if (document.tcpip.pppUserName.value=="") {
          alert('PPP user name cannot be empty!');
	  document.tcpip.pppUserName.value = document.tcpip.pppUserName.defaultValue;
	  document.tcpip.pppUserName.focus();
	  return false;
      }
      if (document.tcpip.pppPassword.value=="") {
          alert('PPP password cannot be empty!');
	  document.tcpip.pppPassword.value = document.tcpip.pppPassword.defaultValue;
	  document.tcpip.pppPassword.focus();
	  return false;
      }
      return true;
  }
  return false;
}

function pppTypeSelection()
{
	
  if ( document.tcpip.pppConnectType.selectedIndex == 2) {
  	if (pppConnectStatus==0) {
  		enableButton(document.tcpip.pppConnect);
		disableButton(document.tcpip.pppDisconnect);
	}
	else {
 		disableButton(document.tcpip.pppConnect);
		enableButton(document.tcpip.pppDisconnect);
	}
	disableTextField(document.tcpip.pppIdleTime);
  }
  else {
	disableButton(document.tcpip.pppConnect);
	disableButton(document.tcpip.pppDisconnect);
	if (document.tcpip.pppConnectType.selectedIndex == 1)
		enableTextField(document.tcpip.pppIdleTime);
	else
		disableTextField(document.tcpip.pppIdleTime);
  }

}

function enableTextField (field) {
  if (document.all || document.getElementById)
    field.disabled = false;
  else {
    field.onfocus = field.oldOnFocus;
  }
}
function disableButton (button) {
  if (document.all || document.getElementById)
    button.disabled = true;
  else if (button) {
    button.oldOnClick = button.onclick;
    button.onclick = null;
    button.oldValue = button.value;
    button.value = 'DISABLED';
  }
}
function disableTextField (field) {
	if (document.all || document.getElementById)
		field.disabled = true;
	else {
		field.oldOnFocus = field.onfocus;
		field.onfocus = skip;
	}
}

function onclick_enable_dhcpv6pd(checked)
{
	with(document.tcpip)
	{
		if(checked)
		{
			enable_dhcpv6pd.value=1;
			
		}else
		{
			enable_dhcpv6pd.value=0
			
		}
	}
}

function originTypeSelection(field)
{
	
	if(field.selectedIndex == 0){//DHCP
		document.getElementById('pppoe_div').style.display = 'none';
		document.getElementById('static_div').style.display = 'none';
		document.getElementById('dns_div').style.display = 'none';
		document.getElementById('dhcp_div').style.display = 'block';
		
		
		if(document.tcpip.elements["dhcpMode"][1].checked == true)
		{//stateless
			statelessDhcpClicked();
		}else
		{//stateful
			statsfulDhcpClicked();
		}
		
	}else if(field.selectedIndex == 1){//static
		document.getElementById('pppoe_div').style.display = 'none';
		document.getElementById('static_div').style.display = 'block';
		document.getElementById('dns_div').style.display = 'block';
		document.getElementById('dhcp_div').style.display = 'none';
		document.tcpip.dnsType.value=0;
		enableDNSinput();
	}
	else if(field.selectedIndex == 2) //PPPoE
	{
		document.getElementById('pppoe_div').style.display = 'block';
		document.getElementById('static_div').style.display = 'none';
		document.getElementById('dns_div').style.display = 'none';
		document.getElementById('dhcp_div').style.display = 'block';
	}
	else if(field.selectedIndex == 3) //DS-Lite
	{
		document.getElementById('pppoe_div').style.display = 'none';
		document.getElementById('static_div').style.display = 'none';
		document.getElementById('dns_div').style.display = 'none';
		document.getElementById('dhcp_div').style.display = 'none';
	}
}

function ipv6_enable_wan_all()
{
	document.tcpip.wan_enable.value=1;
	ipv6WanEnable = 1;
	Load_Setting();
}
function ipv6_disable_wan_all()
{
	document.getElementById('connectionType').style.display = 'none';
	document.getElementById('pppoe_div').style.display = 'none';
	document.getElementById('static_div').style.display = 'none';
	document.getElementById('dns_div').style.display = 'none';
	document.getElementById('dhcp_div').style.display = 'none';
	
}

function ipv6_wan_enable_select(checked)
{
	if(checked)
		ipv6_enable_wan_all();
	else
		ipv6_disable_wan_all();
}

function Load_Setting()
{
	
	var ipv6Origin = <% getIPv6WanInfo("ipv6Origin"); %>;
	//0:DHCP 1:STATIC

	var ipv6LinkType = <%getIPv6WanInfo("ipv6LinkType");%>;
	//0:IP 1:PPP
	
	var wanDnsAuto = <%getIPv6WanInfo("wan_ipv6DnsAuto");%>; //DNS自动手动
	var dhcpMode=<%getIPv6WanInfo("wan_ipv6DhcpMode")%>; //有无状态

	if(ipv6WanEnable)
	{
		document.tcpip.wan_enable.checked=true;
		document.tcpip.wan_enable.value=1;
		document.getElementById('connectionType').style.display = 'block';
	}
	else
	{
		document.tcpip.wan_enable.checked=false;
		document.tcpip.wan_enable.value=0;
		ipv6_disable_wan_all();
		return;
	}
	
	//for dns
	if(wanDnsAuto)
	{
		document.tcpip.elements["dnsMode"][0].checked = true;
		//disableDNSinput();
	}else
	{
		document.tcpip.elements["dnsMode"][1].checked = true;
		//enableDNSinput();
	}
	if(dhcpMode==0)
	{
		document.tcpip.elements["dhcpMode"][1].checked = true;
		statelessDhcpClicked();

	}else
	{
		document.tcpip.elements["dhcpMode"][0].checked = true;
		statsfulDhcpClicked();
	}
	
	if(ipv6Origin == 0 && ipv6LinkType == 0){ //DHCP
		document.tcpip.OriginType.selectedIndex = 0;
	}else if(ipv6Origin == 1 && ipv6LinkType == 0){ //STATIC
		document.tcpip.OriginType.selectedIndex = 1;
	}else if(ipv6Origin == 0 && ipv6LinkType == 1){ // PPPoE
		document.tcpip.OriginType.selectedIndex = 2;
	}
	
	originTypeSelection(document.tcpip.OriginType);
	

	document.tcpip.enable_dhcpv6pd.checked = <%getIPv6WanInfo("enable_dhcpv6pd")%>;
	document.tcpip.enable_dhcpv6RapidCommit.checked = <%getIPv6WanInfo("enable_dhcpv6RapidCommit")%>;
	onclick_enable_dhcpv6pd(document.tcpip.enable_dhcpv6pd.checked);
	
	//for mldproxy
	if ( <% getIPv6WanInfo("mldproxyDisabled"); %>)
		document.tcpip.mldproxyEnabled.checked = false;
	else
		document.tcpip.mldproxyEnabled.checked = true;


	if ( <% getIndex("pppConnectStatus"); %> )
		pppConnectStatus = 1;

}

function multiWanIpv6_saveChanges(form)
{
	if(!form.wan_enable.checked)
	{
		//save nothing
		return true;
	}
	
	if(document.tcpip.OriginType.selectedIndex == 0){ //DHCP
		
	}else if(document.tcpip.OriginType.selectedIndex == 1){ //static
		with(document.tcpip)
		{
			if(!checkIpv6Addr(wan_ip_0,wan_ip_1,wan_ip_2,wan_ip_3,wan_ip_4,wan_ip_5,
				wan_ip_6,wan_ip_7,prefix_len_ip,ipv6_wan_invalid_static_ip))
				return false;
			if(!checkIpv6Addr(wan_gw_0,wan_gw_1,wan_gw_2,wan_gw_3,wan_gw_4,wan_gw_5,
				wan_gw_6,wan_gw_7,prefix_len_gw,ipv6_wan_invalid_gw))
				return false;			
		}
	}else if(document.tcpip.OriginType.selectedIndex == 2){ //PPPoE
//		with(document.tcpip)
//		{
//			if(!checkFieldEmpty(pppUserName,ipv6_wan_pppoe_alert1))
//				return false;
//			if(!checkFieldEmpty(pppPassword,ipv6_wan_pppoe_alert2))
//				return false;
//			if(!checkFieldDigitRange(pppMtuSize,1360,1492,ipv6_wan_invalid_pppoe_mtu))
//				return false;
//		}
	}

	return true;
}

</script>

</head>

<body onload="Load_Setting();">

<div class="int int_2">
	<h1>广域网接口设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
				<li class="on"><a href="ipv6_wan.asp">ipv6 WAN设置</a></li>
				<li><a href="dhcp6s.asp">ipv6 LAN设置</a></li>
				<li><a href="dslite.asp">DS-Lite</a></li>
            </ul>
        </div>
        <div class="cfgright" style="padding-left: 0;width: 498px;">
        	
<form action=/boafrm/frmWanIpv6Setup method=POST name="tcpip">

<table border="0" width=480>
  	<tr>
		<td width="30%" height=40><span>IPv6开关</span></td>
		<td width="70%">
			<input name="wan_enable" class="checkinp" type="checkbox" onclick="ipv6_wan_enable_select(this.checked);">
		</td>
	</tr>    
</table>

<div id="connectionType" class="off">
	<table border="0" width=480>
		<tr>
			<td width="30%" height=40><span>接入方式</span></td>
			<td width="70%">
				<select class="selectinp" size="1" name="OriginType" onChange="originTypeSelection(this)">
					<option value="0">DHCP</option>
					<option value="1">静态IP</option>
					<option value="2">PPPoE</option>
				</select>
			</td>
		</tr>    
	</table>
</div>

<div id="pppoe_div"  class="off">
	<table border="0" width=480>
		<tr>
			<td width="30%" style="height: 14px;"><span></span></td>
			<td width="70%" style="height: 14px;">
				<b style="font-size: 12px;color: #ff7428;">PPPoE账号密码需在互联网设置界面配置</b>
			</td>
		</tr>
		<tr>
			<td width="30%"><span>用户名</span></td>
			<td width="70%">
				<input class="inp1" disabled="disabled" type="text" name="pppUserName" size="18" maxlength="128" value="<% getInfo("pppUserName"); %>">
			</td>
		</tr>
		<tr>
			<td width="30%"><span>密码</span></td>
			<td width="70%">
				<input class="inp1" disabled="disabled" type="text" name="pppPassword" size="18" maxlength="128" value="<% getInfo("pppPassword"); %>">
			</td>
		</tr>
		<tr style="display: none;">
			<td width="30%"><span>服务名</span></td>
			<td width="70%">
				<input class="inp1" type="text" name="pppServiceName" size="18" maxlength="40" value="<% getInfo("pppServiceName"); %>">
			</td>
		</tr>
		<tr style="display: none;">
			<td width="30%"><span>AC名</span></td>
			<td width="70%">
				<input class="inp1" type="text" name="pppAcName" size="18" maxlength="40" value="<% getInfo("pppAcName"); %>">
			</td>
		</tr>
		<tr style="display: none;">
			<td width="30%"><span>连接类型</span></td>
			<td width="70%">
				<select class="selectinp" size="1" name="pppConnectType" onChange="pppTypeSelection()">
					<option selected value="0">连续</option>
					<option value="1">需要时连接</option>
					<option value="2">手动</option>
				</select>&nbsp;&nbsp;
				<input type="submit" value="Connect" name="pppConnect" onClick="return pppConnectClick(0)">&nbsp;&nbsp;
				<input type="submit" value="Disconnect" name="pppDisconnect" onClick="return pppConnectClick(1)">
			</td>
		</tr>
		<tr style="display: none;">
			<td width="30%"><span>空闲时间</span></td>
			<td width="70%">
				<input type="text" class="inp2" name="pppIdleTime" size="10" maxlength="10" value="<% getInfo("wan-ppp-idle"); %>">&nbsp;(1-1000分钟)
			</td>
		</tr>
		<tr>
			<td width="30%"><span>MTU大小</span></td>
			<td width="70%">
				<input type="text" disabled="disabled" class="inp1" name="pppMtuSize" size="10" maxlength="10" value="<% getInfo("pppMtuSize"); %>">&nbsp;
			</td>
		</tr>
	</table>
</div>
 <!-- end of pppoe -->  
 
<div id="static_div"  class="off">
	<table cellSpacing=1 cellPadding=2 border=0>
		<tr>
			<td width="143"><span>IP地址</span></td>
			<td>
				<input class="ipv6Input" type="text" name="wan_ip_0" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Addr_0"); %>">:
				<input class="ipv6Input" type="text" name="wan_ip_1" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Addr_1"); %>">:
				<input class="ipv6Input" type="text" name="wan_ip_2" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Addr_2"); %>">:
				<input class="ipv6Input" type="text" name="wan_ip_3" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Addr_3"); %>">:
				<input class="ipv6Input" type="text" name="wan_ip_4" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Addr_4"); %>">:
			</td>
		</tr>
		<tr>
			<td width="143"><span></span></td>
			<td>
				
				<input class="ipv6Input" type="text" name="wan_ip_5" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Addr_5"); %>">:
				<input class="ipv6Input" type="text" name="wan_ip_6" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Addr_6"); %>">:
				<input class="ipv6Input" type="text" name="wan_ip_7" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Addr_7"); %>">/
				<input class="ipv6Input" type=text name=prefix_len_ip size=4 maxlength=4 enable value=<% getIPv6WanInfo( "wan_ipv6Addr_prefixLen"); %>>
			</td>
		</tr>
		<tr>
			<td ><span>默认网关</span></td>
			<td >
				<input class="ipv6Input" type="text" name="wan_gw_0" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Gw_0"); %>">:
				<input class="ipv6Input" type="text" name="wan_gw_1" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Gw_1"); %>">:
				<input class="ipv6Input" type="text" name="wan_gw_2" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Gw_2"); %>">:
				<input class="ipv6Input" type="text" name="wan_gw_3" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Gw_3"); %>">:
				<input class="ipv6Input" type="text" name="wan_gw_4" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Gw_4"); %>">:
			</td>
		</tr>
		<tr>
			<td ><span></span></td>
			<td >
				
				<input class="ipv6Input" type="text" name="wan_gw_5" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Gw_5"); %>">:
				<input class="ipv6Input" type="text" name="wan_gw_6" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Gw_6"); %>">:
				<input class="ipv6Input" type="text" name="wan_gw_7" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Gw_7"); %>">/
				<input class="ipv6Input" type=text name=prefix_len_gw size=4 maxlength=4 enable value=<% getIPv6WanInfo( "wan_ipv6Gw_prefixLen"); %>>
			</td>
		</tr>
	</table>
</div>
   
 
<div id="dhcp_div" class="off">
	<table cellSpacing=1 cellPadding=2 border=0>
		<tr>
			<td width="143"><span>地址自动配置</span><input type="hidden" name="dhcpType"></td>
			<td >
				<input type="radio" value="stateful" name="dhcpMode" id="dhcpMode1" onClick="statsfulDhcpClicked()"><label for="dhcpMode1">有状态</label> 
				<span><input type="radio" value="stateless" name="dhcpMode" id="dhcpMode2" onClick="statelessDhcpClicked()"><label for="dhcpMode2">无状态</label> </span>
			</td>
		</tr>
		<tr id="pd_div">
			<td width="143"><span>PD开启</span></td>
			<td>
				<input class="checkinp" type="checkbox" name="enable_dhcpv6pd" size=32 maxlength=32 value="on" onclick="onclick_enable_dhcpv6pd(this.checked)">
			</td>
		</tr>
		<tr style="display: none;">
			<td bgColor=#aaddff><span>快速分配开启</span></td>
			<td bgColor=#ddeeff>
				<input class="checkinp" type="checkbox" name="enable_dhcpv6RapidCommit" size=32 maxlength=32 value="on">
			</td>
		</tr>
	</table>
</div>


<div id="6rd_div" class="off" style="display: none;">
	<table cellSpacing=1 cellPadding=2 border=0>
		<h3><font color="#0000FF">6RD</font></h3>
		<tr>
			<td bgColor=#aaddff><font size=2><b><script>dw(ipv6_wan_6rd_ipv6_prefix)</script></b></td>
			<td bgColor=#aaddff><script>dw(ipv6_wan_prefix)</script></td>
		</tr>
		<tr class="dnsinpt">
			<td bgColor=#ddeeff>
				<input type="text" name="IPv6_6rd_ip_0" size="4" maxlength="4" value="<% getIPv6WanInfo("6rd_ipv6Addr_0"); %>">:
				<input type="text" name="IPv6_6rd_ip_1" size="4" maxlength="4" value="<% getIPv6WanInfo("6rd_ipv6Addr_1"); %>">:
				<input type="text" name="IPv6_6rd_ip_2" size="4" maxlength="4" value="<% getIPv6WanInfo("6rd_ipv6Addr_2"); %>">:
				<input type="text" name="IPv6_6rd_ip_3" size="4" maxlength="4" value="<% getIPv6WanInfo("6rd_ipv6Addr_3"); %>">:
				<input type="text" name="IPv6_6rd_ip_4" size="4" maxlength="4" value="<% getIPv6WanInfo("6rd_ipv6Addr_4"); %>">:
				<input type="text" name="IPv6_6rd_ip_5" size="4" maxlength="4" value="<% getIPv6WanInfo("6rd_ipv6Addr_5"); %>">:
				<input type="text" name="IPv6_6rd_ip_6" size="4" maxlength="4" value="<% getIPv6WanInfo("6rd_ipv6Addr_6"); %>">:
				<input type="text" name="IPv6_6rd_ip_7" size="4" maxlength="4" value="<% getIPv6WanInfo("6rd_ipv6Addr_7"); %>">
			</td>
			<td bgColor=#ddeeff>
				<input type=text name=prefix_len_6rd size=4 maxlength=4 enable value=<% getIPv6WanInfo( "6rd_ipv6Addr_prefixLen"); %>>
			</td>
		</tr>
		<tr>
			<td bgColor=#aaddff><font size=2><b><script>dw(ipv6_wan_ipv4_addr)</script></b></td>
			<td bgColor=#aaddff><script>dw(ipv6_wan_mask_len)</script></td>
		</tr>
		<tr>
			<td bgColor=#ddeeff>
				<input type="text" class="inp3" name="wanIP" size="18" maxlength="15" disabled="disabled" value="<% getIPv6WanInfo("6rd_wan_ip"); %>">
			</td>
			<td bgColor=#ddeeff>
				<input type=text name="wanMask" size=4 maxlength=4 enable value="<% getIPv6WanInfo("6rd_ipv4Addr_netMask"); %>">
			</td>
		</tr>
		<tr>
			<td bgColor=#aaddff><font size=2><b><script>dw(ipv6_wan_6rd_border_relay_ipv4)</script></b></td>
		</tr>
		<tr>
			<td bgColor=#ddeeff>
				<input type="text" class="inp3" name="IPv6_6rd_BR_IP" size="18" maxlength="15" value=<% getIPv6WanInfo( "6rd_BR_IP"); %>>
			</td>
		</tr>
	</table>
</div>

<div id="dns_head_div" class="off" style="display: none;">
	<h3><font color="#0000FF"><script>dw(ipv6_wan_dns_set)</script></font></h3>
	<BR>
    <b><script>dw(ipv6_wan_dnsv6)</script></b><BR>
</div>	
<div id="dns_mode_div" class="off" style="display: none;">
	<table cellSpacing=1 cellPadding=2 border=0>
		<tr>
			<input type="hidden" name="dnsType">
			<td width="100%" colspan="2">
				<font size=2>
					<b><input type="radio" value="dnsAuto" name="dnsMode" onClick="autoDNSclicked()"><script>dw(ipv6_wan_auto_dns)</script></b>
			</td>
		</tr>
		<tr>
			<td width="100%" colspan="2">
				<font size=2>
					<b><input type="radio" value="dnsManual" name="dnsMode" onClick="manualDNSclicked()"><script>dw(ipv6_wan_manually_dns)</script></b>
			</td>
		</tr>
	</table>
</div>
<div id="dns_div" class="off">
	<table cellSpacing=1 cellPadding=2 border=0>
		<tr>
			<td width="143"><span>DNS</span></td>
			<td >
				<input class="ipv6Input" type="text" name="wan_dns1_0" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Dns1_0"); %>">:
				<input class="ipv6Input" type="text" name="wan_dns1_1" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Dns1_1"); %>">:
				<input class="ipv6Input" type="text" name="wan_dns1_2" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Dns1_2"); %>">:
				<input class="ipv6Input" type="text" name="wan_dns1_3" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Dns1_3"); %>">:
				<input class="ipv6Input" type="text" name="wan_dns1_4" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Dns1_4"); %>">:
			</td>
		</tr>
		<tr>
			<td width="143"><span></span></td>
			<td >
				
				<input class="ipv6Input" type="text" name="wan_dns1_5" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Dns1_5"); %>">:
				<input class="ipv6Input" type="text" name="wan_dns1_6" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Dns1_6"); %>">:
				<input class="ipv6Input" type="text" name="wan_dns1_7" size="4" maxlength="4" value="<% getIPv6WanInfo("wan_ipv6Dns1_7"); %>">/
				<input class="ipv6Input" type=text name=prefix_len_dns1 size=4 maxlength=4 enable value=<% getIPv6WanInfo( "wan_ipv6Dns1_prefixLen"); %>>
			</td>
		</tr>
	</table>
</div>
        
<div id="other_div" class="off" style="display: none;">
	<table border="0" width=480>
		<h3><font color="#0000FF"><script>dw(ipv6_wan_other)</script></font></h3>
		<tr style="display: none;">
			<td width="30%">
				<font size=2><b><script>dw(ipv6_wan_clone_mac)</script></b></td>
			<td width="70%">
				<input type="text" class="inp3" name="wan_macAddr" size="18" maxlength="12" value=<% getInfo( "wanMac"); %>>
			</td>
		</tr>
		<tr>
			<td width="100%" colspan="2">
				<b><input class="checkinp" type="checkbox" name="mldproxyEnabled" value="ON">&nbsp;&nbsp;<script>dw(ipv6_wan_mld)</script></b>
			</td>
		</tr>
	</table>
</div>

<input type="hidden" value="/herouter/ipv6_wan.asp" name="submit-url">   
<div class="subpart">
	<ul>
		<li><b onclick="ipv6Submit()">保存</b></li>
		<li><b class="cancel" onclick="javascript:parent.TINY.box.hide();">取消</b></li>
		
	</ul>
</div>
</form>
        </div>
    </div>
</div>
<script type="text/javascript">
	function ipv6Submit(){
		if(multiWanIpv6_saveChanges(document.tcpip) == true){
			//提示框
			Alert('正在保存设置，请稍后！',20);
			document.tcpip.submit();
		}else{
			return false;
		}
	}
</script>

</body>
</html>
