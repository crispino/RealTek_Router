<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<!--系统默认模板-->
<HTML>
<HEAD>
<TITLE>PING测试</TITLE>
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

/********************************************************************
**          on document load
********************************************************************/
function isValidUrlName(url)
			{
				var i=0;
				var invalidArray = new Array();
				invalidArray[i++] = "www";
				invalidArray[i++] = "com";
				invalidArray[i++] = "org";
				invalidArray[i++] = "net";
				invalidArray[i++] = "edu";
				invalidArray[i++] = "www.";
				invalidArray[i++] = ".com";
				invalidArray[i++] = ".org";
				invalidArray[i++] = ".net";
				invalidArray[i++] = ".edu";
				if (url == "")
				{
					return false;
				}
				for (i = 0; i < url.length; i++)
				{
					if (url.charAt(i) == '\\')
					{
						return false;
					}
				}			
				if (url.length < 3)
				{
					return false;
				}
				for(j=0; j< invalidArray.length; j++)
				{
					if (url == invalidArray[j])
					{
						return false;
					}
				}
				return true;
			} 

function on_Apply(){
	var url = document.forms[0].target_addr.value;

	if(!isValidUrlName(url) || !sji_checkvip(url) && !sji_checkhostname(url)
		&& !isIPv6(url)){
		alert("目标地址非法！请重新输入！");
		document.forms[0].target_addr.value="";
		return false;
	}
	if(document.forms[0].target_num.value > 0 && document.forms[0].target_num.value <= 3)
	{
		//document.forms[0].submit();
		return true;
	}
	else
	{
		alert("重复次数非法！请重新输入！");
		document.forms[0].target_num.focus();
		return false;
	}
}
</SCRIPT>
</HEAD>

<!-------------------------------------------------------------------------------------->
<!--主页代码-->
<body topmargin="0" leftmargin="0" marginwidth="0" marginheight="0" alink="#000000" link="#000000" vlink="#000000">
	<blockquote>
	<form id="form" action=/boafrm/formPing method=POST target="pingresult" >
		<div align="left" style="padding-left:20px;"><br>
			<div align="left"><b>PING测试</b></div>
			<br>
			<table width="50%" align="left" valign="middle">
			<tr><td>重复次数：<input type="text" name="target_num" maxlength="60" value=1>(1-3次)</td></tr>
			<tr><td>接口选择：<select name="wanifac"><option value="default">Default</option> 
													<% getInfo("multi_wan_name_get"); %></select></td></tr>
			<tr><td>目标地址：<input type="text" name="target_addr" maxlength="60" value="127.0.0.1"/>
			<button class="btnsaveup" type="submit" onClick="return on_Apply();">开始</button></td></tr>
			</table>
		</div>
		<div align="left"><br>
		<iframe align="left" id="pingresult" name="pingresult" src='' frameborder='0' width='600' height='800'></iframe>
		</div>
	</form>
	</blockquote>
</body>
</html>
