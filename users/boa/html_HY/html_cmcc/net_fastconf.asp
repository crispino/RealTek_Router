<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<!--系统默认模板-->
<html>
<HEAD>
<TITLE>快速配置向导</TITLE>
<META http-equiv=pragma content=no-cache>
<META http-equiv=cache-control content="no-cache, must-revalidate">
<META http-equiv=Content-Language content=zh-cn>
<META http-equiv=content-type content="text/html; charset=utf-8">
<!--系统公共css-->
<STYLE type=text/css>
@import url(/style/default.css);
</STYLE>
<style>
TABLE{width:100%;}
TR{height:16px;}
SELECT {width:150px;}
</style>
<!--系统公共脚本-->
<SCRIPT language="javascript" src="common.js"></SCRIPT>
<script type="text/javascript" src="share_cmcc.js"></script>
<script>
var ipmodeIdx = 0;
var pppUserName="";
var pppPassword="";
var pppServiceName="";

<% getInfo("initPageWizard");%>

function saveChanges()
{
	var form1 = document.formfastconf;

	if (form1.ipmode[0].checked == true)  //DHCP mode
	{
		/*if((internetWanIdx >= 0) && (ipmodeIdx == 0))
		{
			alert('没有修改，不保存!');
			return false;
		}*/
		return true;
	}
	
	if (document.formfastconf.username.disabled == false) {
		if((ipmodeIdx == 1) && (pppUserName == document.formfastconf.username.value) 
			&& (pppPassword == document.formfastconf.password.value))
		{
			alert('没有修改，不保存!');
			return false;
		}
		
		if (document.formfastconf.username.value=="") {
			alert('PPP 用户名不能为空!');
			document.formfastconf.username.focus();
			return false;
		}
		if (includeSpace(document.formfastconf.username.value)) {
			alert('PPP 用户名中不能有空格!');
			document.formfastconf.username.focus();
			return false;
		}
		if (checkString(document.formfastconf.username.value) == 0) {
			alert('无效的 PPP 用户名!');
			document.formfastconf.username.focus();
			return false;
		}

		if (document.formfastconf.password.value=="") {
			alert('PPP 密码不能为空!');
			document.formfastconf.password.focus();
			return false;
		}
		if (includeSpace(document.formfastconf.password.value)) {
			alert('PPP 密码中不能有空格!');
			document.formfastconf.password.focus();
			return false;
		}
		if (checkString(document.formfastconf.password.value) == 0) {
			alert('无效的 PPP 密码!');
			document.formfastconf.password.focus();
			return false;
		}

		if (includeSpace(document.formfastconf.password.value)) {
			alert('PPP 服务名称中不能有空格!');
			document.formfastconf.serviceName.focus();
			return false;
		}
		if (checkString(document.formfastconf.password.value) == 0) {
			alert('无效的 PPP 服务名称!');
			document.formfastconf.serviceName.focus();
			return false;
		}
	}
	
	return true;
}
function on_ctrlupdate()
{
	with ( document.forms[0] ) 
	{
		tbdial.style.display = "none";
		if (ipmode[0].checked == true)  //DHCP, IPv4 only, or IPv4/IPv6
		{	
			tbdial.style.display = "none";
		}else if (ipmode[1].checked == true) //PPPoE, IPv4 only, or IPv4/IPv6, or IPv6 only
		{	
			tbdial.style.display = "block";
		}	
	}
}

function on_init()
{
	with ( document.forms[0] ) 
	{
		connType[ipmodeIdx].checked = true;
		
		ipmode[ipmodeIdx].checked = true;
		username.value = password.value = "";
		serviceName.value = "";
		
		if(internetWanIdx >= 0)
		{
			wanIdx.value = internetWanIdx;
			if(ipmodeIdx == 1)
			{
				username.value = pppUserName;
				password.value = pppPassword;
				serviceName.value = pppServiceName;
			}	
		}
		else
		{
			wanIdx.value = -1;
		}
		on_ctrlupdate();
	}
}
</script>
</head>

<body topmargin="0" leftmargin="0" marginwidth="0" marginheight="0" alink="#000000" link="#000000" vlink="#000000"  onLoad="on_init();">
<blockquote>
<div align="left" style="padding-left:20px; padding-top:5px">
	<div class="tip" style="width:90% ">
		<b>快速配置</b><br><br>
		本页允许您快速添加或修改 Internet 连接.
	</div>
	<hr align="left" class="sep" size="1" width="90%">
	<br>
	<form action=/boafrm/formFastConf method=POST name="formfastconf">
		<table border=0 width="500" cellspacing=0 cellpadding=0>
		  <tr>
		      <td width="150px"><b>上网方式自动侦测结果:</b></td>
		      <td>
		      	<input type="radio" value="0" name="connType" disabled="disabled">DHCP&nbsp;&nbsp;
		     	<input type="radio" value="1" name="connType" disabled="disabled">PPPoE&nbsp;&nbsp;
		      </td>
		  </tr>
		</table>

		<table cellpadding="0px" cellspacing="2px">
			<tr class="sep"><td colspan="2"><hr align="left" class="sep" size="1" width="100%"></td></tr>
			<tr nowrap><td width="150px"><input type="radio" id="ipmode"  name="ipmode" value="0" onClick="on_ctrlupdate()">DHCP</td><td >从ISP处自动得到一个地址</td></tr>
		</table>	

		<table  cellpadding="0px" cellspacing="2px">			
			<tr class="sep"><td colspan="2"><hr align="left" class="sep" size="1" width="100%"></td></tr>
			<tr nowrap><td width="150px"><input type="radio" id="ipmode"  name="ipmode" value="2" onClick="on_ctrlupdate()">PPPoE </td><td>若使用PPPOE请选择此项</td></tr>
		</table>	

		<table id="tbdial" cellpadding="0px" cellspacing="2px">
			<tr class="sep"><td colspan="2"><hr align="left" class="sep" size="1" width="100%"></td></tr>
			<tr nowrap>
				<td width="150px">用户名：</td>
				<td><input type="text" name="username" maxlength="30" size="16" style="width:150px "></td>
			</tr>
			<tr nowrap>
				<td>密码：</td>
				<td><input type="password"name="password" maxlength="30" size="16" style="width:150px "></td>
			</tr>
			<tr nowrap>
				<td>服务名称：</td>
				<td><input type="text" name="serviceName" maxlength="20" size="16" style="width:150px "></td>
			</tr>
		</table> 

		<input type="submit" value="应用"  name="save"  onClick="return saveChanges()" class="link_bg"></td>
		<input type="hidden" name="wanIdx" value="">
		<input type="hidden" value="/tcpipwan.htm" name="submit-url">
	</form>
</div>
</body>
</html>

