<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>DS-Lite </title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<script type="text/javascript" src="js/util_gw.js"> </script>
<SCRIPT language=Javascript src="js/language_sc.js"></SCRIPT>
<link href="css/style.css" rel="stylesheet" type="text/css">
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<style type="text/css">
.ipv6Input{
	font-size: 14px;margin-bottom: 4px;
	height: 26px;
    border: 1px #d2d2d2 solid;
    width: 40px;
    font-family: "微软雅黑";
    color: #666;
    padding: 0 8px;
}
.cfg td label {
    line-height: 36px;
}
</style>
<script>

function autoDsliteclicked()
{
	document.tcpip.dsliteAftrIpAddr6.disabled = true;
	document.getElementById('aftr_div').style.display = 'none';
}

function manualDsliteclicked()
{
	document.getElementById('aftr_div').style.display = '';
	document.tcpip.dsliteAftrIpAddr6.disabled = false;
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


function ipv6_dslite_enable_select(checked)
{
	if(checked)
		document.tcpip.dslite_enable.value=1;
	else
		document.tcpip.dslite_enable.value=0;
}

function Load_Setting()
{

	if ( <% getIndex("wanDhcp"); %> == 17) {
		document.tcpip.dslite_enable.checked=true;
		document.tcpip.dslite_enable.value=1;
	}else{
		document.tcpip.dslite_enable.checked=false;
		document.tcpip.dslite_enable.value=0;
	}
	
	if ( <% getIndex("dsliteMode"); %> == 0) {
		document.tcpip.elements["dsliteMode"][0].checked = true;
		autoDsliteclicked();
	}else{
		document.tcpip.elements["dsliteMode"][1].checked = true;
		manualDsliteclicked();
	}
	
}

function multiWanIpv6_saveChanges(form)
{
	if(document.tcpip.dslite_enable.value == 1 && $("input[name='dsliteMode']:checked").val() == 'dsliteManual'){
		if(!checkIPv6(document.tcpip.dsliteAftrIpAddr6.value)){
			alert("请输入正确的ipv6地址");
			document.tcpip.dsliteAftrIpAddr6.focus();
			return false;
		}
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
				<li><a href="ipv6_wan.asp">ipv6 WAN设置</a></li>
				<li><a href="dhcp6s.asp">ipv6 LAN设置</a></li>
				<li class="on"><a href="dslite.asp">DS-Lite</a></li>
            </ul>
        </div>
        <div class="cfgright" style="padding-left: 0;">
        	
<form action=/boafrm/frmIpv6DsliteSetup method=POST name="tcpip">  

<table border="0" width=480>
  	<tr>
		<td width="30%" height=40><span><script>dw(ipv6_dslite_enable)</script></span></td>
		<td width="70%">
			<input name="dslite_enable" class="checkinp" type="checkbox" onclick="ipv6_dslite_enable_select(this.checked);">
		</td>
	</tr>    
</table>

<div id="dslite_div" >
	<table cellSpacing=1 cellPadding=2 border=0>
		<tr>
			<td width="143"><span>AFTR获取</span></td>
			<td colspan="2">
				<input type="radio" value="dsliteAuto" name="dsliteMode" id="dsliteMode1" onclick="autoDsliteclicked();"><label for="dsliteMode1">自动</label> 
				<span><input type="radio" value="dsliteManual" name="dsliteMode" id="dsliteMode2" onclick="manualDsliteclicked();"><label for="dsliteMode2">手动</label> </span>
			</td>
		</tr>
		<tr id="aftr_div" style="display: none;">
			<td width="143"><span>AFTR IPv6地址</span></td>
			<td>
				<input class="inp1" style="width: 248px;" type="text" name="dsliteAftrIpAddr6" size="40" maxlength="40" value="<% getInfo("dsliteAftr"); %>">
			</td>
		</tr>
		<tr style="display: none;">
	      <td width="30%"><font size=2><b>WAN IPv6地址</b></td>
	      <td width="70%"><font size=2><input type="text" name="dsliteWanIpAddr6" size="40" maxlength="40" disabled="disabled" value="<% getInfo("ipv6WanIp"); %>"></td>
	    </tr>
	    <tr style="display: none;">
	      <td width="30%"><font size=2><b>默认网关</b></td>
	      <td width="70%"><font size=2><input type="text" name="dsliteDefGw" size="40" maxlength="40" disabled="disabled" value="<% getInfo("ipv6DefGW"); %>"></td>
	    </tr>
	</table>
</div>




<input type="hidden" value="/herouter/dslite.asp" name="submit-url">   
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
