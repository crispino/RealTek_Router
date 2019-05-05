<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>基本信息</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<style type="text/css">
<!--
body{ color:#676767;}
td.tbl_title {
    background-color: #3B9DCC;
    color: #FFFFFF;
    span-weight: bold;
    span-size: 18px;
}
td{
	span-size: 14px;
	line-height: 28px;
	padding-left: 16px;
}
-->
</style>


<script type="text/javascript">
function initWorkingMode()
{
	var opmode = '<% getIndex("opMode"); %>';
	var repeaterMode = '<% getHeInfo("repeaterMode"); %>'; 

	if(opmode == 0){
		$("#workingMode").text("路由模式");
	}else if(opmode==1) {
		if (repeaterMode != 1) {
			$("#workingMode").text("桥接模式");
		} else {
			$("#workingMode").text("路由模式");
		}
	} else {
		$("#workingMode").text("未知模式");
	}
	
	return;
}

function initTelnet()
{
	var telnet = '<%getHeInfo("lanTelnet"); %>';
	if (telnet == 1) {
		$("#telnet").text("关闭");
	} else {
		$("#telnet").text("打开");
	}
}

$(document).ready(function(){

	initWorkingMode();
	
	initTelnet();
});
</script>  
</head>

<body >
<form>
<table width="600" border="0&quot;" style="margin: 50px auto;">
	<tbody>
		<tr>
			<td width="100%" colspan="2" class="tbl_title">基本信息</td>
		</tr>
		<tr bgcolor="#EEEEEE">
			<td width="40%">
				<span ><b>工作模式</b></span>
			</td>
			<td width="60%">
				<span id="workingMode"><%getHeInfo("workingMode"); %></span>
			</td>
		</tr>
		<tr bgcolor="#DDDDDD">
			<td width="40%">
				<span ><b>MAC地址</b></span>
			</td>
			<td width="60%">
				<span ><%getInfo("hwaddr"); %></span>
			</td>
		</tr>
		<tr bgcolor="#EEEEEE">
			<td width="40%">
				<span ><b>IPv4地址</b></span>
			</td>
			<td width="60%">
				<span ><% getInfo("ip"); %> / <% getInfo("mask"); %></span>
			</td>
		</tr>
		<tr bgcolor="#DDDDDD">
			<td width="40%">
				<span ><b>软件版本</b></span>
			</td>
			<td width="60%">
				<span ><%getInfo("fwVersion");%></span>
			</td>
		</tr>
		<tr bgcolor="#EEEEEE">
			<td width="40%">
				<span ><b>SVN版本</b></span>
			</td>
			<td width="60%">
				<span ><%getHeInfo("svnVersion");%></span>
			</td>
		</tr>
		<tr bgcolor="#DDDDDD">
			<td width="40%">
				<span ><b>SN码</b></span>
			</td>
			<td width="60%">
				<span ><%getHeInfo("STBID");%></span>
			</td>
		</tr>
		
		<tr bgcolor="#EEEEEE">
			<td width="40%">
				<span ><b>云网关IP</b></span>
			</td>
			<td width="60%">
				<span ><%getHeInfo("cgw_ipaddr");%></span>
			</td>
		</tr>
		<tr bgcolor="#DDDDDD">
			<td width="40%">
				<span ><b>Telnet开关</b></span>
			</td>
			<td width="60%">
				<span id="telnet"></span>
			</td>
		</tr>
		<tr bgcolor="#EEEEEE">
			<td width="40%">
				<span ><b>Website</b></span>
			</td>
			<td width="60%">
				<span ><%getHeInfo("Website"); %></span>
			</td>
		</tr>
		<tr bgcolor="#DDDDDD">
			<td width="40%">
				<span ><b>WEB用户名</b></span>
			</td>
			<td width="60%">
				<span ><%getHeInfo("userName"); %></span>
			</td>
		</tr>
		<tr bgcolor="#EEEEEE">
			<td width="40%">
				<span ><b>2.4G无线SSID</b></span>
			</td>
			<td width="60%">
				<span ><%getHeInfo("SSID", "wlan2")%></span>
			</td>
		</tr>
		<tr bgcolor="#DDDDDD">
			<td width="40%">
				<span ><b>2.4G无线密码</b></span>
			</td>
			<td width="60%">
				<span ><%getHeInfo("pskValue", "wlan2")%></span>
			</td>
		</tr>
		<tr bgcolor="#EEEEEE">
			<td width="40%">
				<span ><b>2.4G访客网络SSID</b></span>
			</td>
			<td width="60%">
				<span ><%getHeInfo("SSID", "wlan2-va0")%></span>
			</td>
		</tr>
		
		<tr bgcolor="#DDDDDD">
			<td width="40%">
				<span ><b>5G无线SSID</b></span>
			</td>
			<td width="60%">
				<span ><%getHeInfo("SSID", "wlan1")%></span>
			</td>
		</tr>
		<tr bgcolor="#EEEEEE">
			<td width="40%">
				<span ><b>5G无线密码</b></span>
			</td>
			<td width="60%">
				<span ><%getHeInfo("pskValue", "wlan1")%></span>
			</td>
		</tr>
		<tr bgcolor="#DDDDDD">
			<td width="40%">
				<span ><b>5G访客网络SSID</b></span>
			</td>
			<td width="60%">
				<span ><%getHeInfo("SSID", "wlan1-va0")%></span>
			</td>
		</tr>
	</tbody>
</table>
</form>
</body>
</html>
