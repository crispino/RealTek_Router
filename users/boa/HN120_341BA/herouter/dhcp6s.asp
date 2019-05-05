<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>局域网设置 </title>
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
</style>
<script>
function onclick_func()
{
	if(document.Dhcpv6form.enable_dhcpv6s.checked == false)
	{
		document.Dhcpv6form.dnsaddr.disabled = true;
		document.Dhcpv6form.interfacenameds.disabled = true;
		document.Dhcpv6form.addrPoolStart.disabled = true;
		document.Dhcpv6form.addrPoolEnd.disabled = true;
	}
	else
	{
		document.Dhcpv6form.dnsaddr.disabled = false;
		document.Dhcpv6form.interfacenameds.disabled = false;
		document.Dhcpv6form.addrPoolStart.disabled = false;
		document.Dhcpv6form.addrPoolEnd.disabled = false;
	}
}

function Save_LANv6()
{
	with(document.Dhcpv6form)
	{
		if(!checkIpv6Addr(lan_ip_0,lan_ip_1,lan_ip_2,lan_ip_3,lan_ip_4,
			lan_ip_5,lan_ip_6,lan_ip_7,prefix_len_lan,dhcp6s_invalid_lan_ipv6))
			return false;


		if(document.Dhcpv6form.enable_dhcpv6s.checked == true)
		{
			if(!checkIPv6(addrPoolStart.value)){
				alert(dhcp6s_invalid_start_ip);
				addrPoolStart.focus();
				return false;
			}

			if(!checkIPv6(addrPoolEnd.value)){
				alert(dhcp6s_invalid_end_ip);
				addrPoolEnd.focus();
				return false;
			}
		}
	}	
	return true;	
}

function init()
{
	var ipv6WanEnable = <%getIPv6WanInfo("ipv6WanEnabled")%>;
	if(ipv6WanEnable)
	{
		document.Dhcpv6form.lan_ip_0.disabled = false;
		document.Dhcpv6form.lan_ip_1.disabled = false;
		document.Dhcpv6form.lan_ip_2.disabled = false;
		document.Dhcpv6form.lan_ip_3.disabled = false;
		document.Dhcpv6form.lan_ip_4.disabled = false;
		document.Dhcpv6form.lan_ip_5.disabled = false;
		document.Dhcpv6form.lan_ip_6.disabled = false;
		document.Dhcpv6form.lan_ip_7.disabled = false;
		document.Dhcpv6form.prefix_len_lan.disabled=false;
		document.Dhcpv6form.enable_dhcpv6s.disabled = false;
		
	    var enable_dhcp6s = <% getIPv6Info("enable_dhcpv6s"); %>;
	    if(enable_dhcp6s)
	    {
			document.Dhcpv6form.enable_dhcpv6s.checked = true;
			document.Dhcpv6form.dnsaddr.disabled = false;
			document.Dhcpv6form.interfacenameds.disabled = false;
			document.Dhcpv6form.addrPoolStart.disabled = false;
			document.Dhcpv6form.addrPoolEnd.disabled = false;
			
		}
		else
		{
			document.Dhcpv6form.enable_dhcpv6s.checked = false;
			document.Dhcpv6form.dnsaddr.disabled = true;
			document.Dhcpv6form.interfacenameds.disabled = true;
			document.Dhcpv6form.addrPoolStart.disabled = true;
			document.Dhcpv6form.addrPoolEnd.disabled = true;
		}

	}else{
		document.Dhcpv6form.lan_ip_0.disabled = true;
		document.Dhcpv6form.lan_ip_1.disabled = true;
		document.Dhcpv6form.lan_ip_2.disabled = true;
		document.Dhcpv6form.lan_ip_3.disabled = true;
		document.Dhcpv6form.lan_ip_4.disabled = true;
		document.Dhcpv6form.lan_ip_5.disabled = true;
		document.Dhcpv6form.lan_ip_6.disabled = true;
		document.Dhcpv6form.lan_ip_7.disabled = true;
		document.Dhcpv6form.prefix_len_lan.disabled=true;

		document.Dhcpv6form.enable_dhcpv6s.disabled = true;
		document.Dhcpv6form.enable_dhcpv6s.checked = false;
		document.Dhcpv6form.dnsaddr.disabled = true;
		document.Dhcpv6form.interfacenameds.disabled = true;
		document.Dhcpv6form.addrPoolStart.disabled = true;
		document.Dhcpv6form.addrPoolEnd.disabled = true;
	}
}
</script>

</head>

<body onload="init();">

<div class="int int_2">
	<h1>局域网设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
				<li><a href="ipv6_wan.asp">ipv6 WAN设置</a></li>
				<li class="on"><a href="dhcp6s.asp">ipv6 LAN设置</a></li>
				<li><a href="dslite.asp">DS-Lite</a></li>
            </ul>
        </div>
        <div class="cfgright"  style="padding-left: 0;width: 498px;">
        	
<form method="POST" action="/boafrm/frmLanIpv6Setup" name="Dhcpv6form">
<table cellSpacing=1 cellPadding=2 border=0>
	<tr>
		<td width="143"><span>IP地址</span></td>
		<td>
			<input class="ipv6Input" type="text" name="lan_ip_0" size="4" maxlength="4" value="<% getIPv6WanInfo("lan_ipv6Addr_0"); %>">:
			<input class="ipv6Input" type="text" name="lan_ip_1" size="4" maxlength="4" value="<% getIPv6WanInfo("lan_ipv6Addr_1"); %>">:
			<input class="ipv6Input" type="text" name="lan_ip_2" size="4" maxlength="4" value="<% getIPv6WanInfo("lan_ipv6Addr_2"); %>">:
			<input class="ipv6Input" type="text" name="lan_ip_3" size="4" maxlength="4" value="<% getIPv6WanInfo("lan_ipv6Addr_3"); %>">:
			<input class="ipv6Input" type="text" name="lan_ip_4" size="4" maxlength="4" value="<% getIPv6WanInfo("lan_ipv6Addr_4"); %>">:
			</td>
	</tr>
	<tr>
		<td width="143"><span></span></td>
		<td>
			
			<input class="ipv6Input" type="text" name="lan_ip_5" size="4" maxlength="4" value="<% getIPv6WanInfo("lan_ipv6Addr_5"); %>">:
			<input class="ipv6Input" type="text" name="lan_ip_6" size="4" maxlength="4" value="<% getIPv6WanInfo("lan_ipv6Addr_6"); %>">:
			<input class="ipv6Input" type="text" name="lan_ip_7" size="4" maxlength="4" value="<% getIPv6WanInfo("lan_ipv6Addr_7"); %>">/	
			<input class="ipv6Input" type="text" name="prefix_len_lan" size="4" maxlength="4"  value="<% getIPv6WanInfo("lan_ipv6Addr_prefixLen"); %>">
		</td>
	</tr>
</table>

<table cellSpacing=1 cellPadding=2 border=0>
	<tr>
		<td width="143"><span>DHCP服务开关</span></td>
		<td ><input class="checkinp" type="checkbox" name="enable_dhcpv6s" value =1 onclick="onclick_func()"></td>
	</tr>
	<tr>
		<td ><span>起始地址</span></td>
		<td ><input class="inp1"  style="width: 248px;" type=text name=addrPoolStart size=48 maxlength=48 value="<% getIPv6Info("addrPoolStart"); %>"></td>
	</tr>
	<tr>
		<td ><span>结束地址</span></td>
		<td ><input class="inp1" style="width: 248px;" type=text name=addrPoolEnd size=48 maxlength=48 value="<% getIPv6Info("addrPoolEnd"); %>"></td>
	</tr>
	<tr style="display: none;">
		<td bgColor=#aaddff>DNS地址</td>
		<td bgColor=#ddeeff><input class="inp1" type="text" name="dnsaddr" size=48 maxlength=48 value="<% getIPv6Info("dnsaddr"); %>"></td>
	</tr>
	<tr style="display: none;">
		<td bgColor=#aaddff>接口名称</td>
		<td bgColor=#ddeeff><input class="inp1" type="text" name="interfacenameds" size=48 maxlength=48 value="<% getIPv6Info("interfacenameds"); %>"></td>
	</tr>
</table>

<input type="hidden" value="/herouter/dhcp6s.asp" name="submit-url">
<div class="subpart">
	<ul>
		<li><b onclick="dhcp6sSubmit()">保存</b></li>
		<li><b class="cancel" onclick="javascript:parent.TINY.box.hide();">取消</b></li>
		
	</ul>
</div>
</form>
        </div>
    </div>
</div>

<script type="text/javascript">
	function dhcp6sSubmit(){
		if(Save_LANv6() == true){
			//提示框
			Alert('正在保存设置，请稍后！',20);
			document.Dhcpv6form.submit();
		}else{
			return false;
		}
	}
</script>
</body>
</html>
