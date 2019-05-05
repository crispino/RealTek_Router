<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>局域网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript">
var secs;
var timerID = null;
var timerRunning = false;

function StartTheTimer(){
	if (secs==0){
		TimeoutReload(5);
		//window.location.reload();
		//alert("http://" + document.lanCfg.lanIp.value);
		//window.location.href = "http://" + document.lanCfg.lanIp.value;	//reload page
    }else{
        self.status = secs
        secs = secs - 1
        timerRunning = true
        timerID = self.setTimeout("StartTheTimer()", 1000)
    }
}

function TimeoutReload(timeout)
{
	secs = timeout;
	if(timerRunning)
		clearTimeout(timerID)
	timerRunning = false
	StartTheTimer();	
}
var oldIp;
function recIpCfg()
{
	oldIp = document.dhcp_setting.lanIp.value;
}

/*
 * Try to modify dhcp server configurations:
 *   dhcp start/end ip address to the same as new lan ip address
 */
function modDhcpCfg()
{
    StartTheTimer();
	var i, j;
	var mask = "<% getInfo("mask-rom"); %>";
	var newNet = document.dhcp_setting.lanIp.value;

	//support simple subnet mask only
	if (mask == "255.255.255.0")
		mask = 3;
	else if (mask == "255.255.0.0")
		mask = 2;
	else if (mask == "255.0.0.0")
		mask = 1;
	else
		return;

	//get the old subnet
	for (i=0, j=0; i<oldIp.length; i++) {
		if (oldIp.charAt(i) == '.') {
			j++;
			if (j != mask)
				continue;
			oldIp = oldIp.substring(0, i);
			break;
		}
	}

	//get the new subnet
	for (i=0, j=0; i<newNet.length; i++) {
		if (newNet.charAt(i) == '.') {
			j++;
			if (j != mask)
				continue;
			newNet = newNet.substring(0, i);
			break;
		}
	}

	stip = new Array;
	stip = document.dhcp_setting.lanIp.value.split(".");
	edip = new Array;
	edip = document.dhcp_setting.lanIp.value.split(".");
	var sturl= stip[0] + "." + stip[1] + "." + stip[2];
	document.dhcp_setting.startip1.value = sturl;
	var edurl= edip[0] + "." + edip[1] + "." + edip[2];
	document.dhcp_setting.endip1.value = edurl;

	//document.dhcp_setting.dhcpStart.value = document.dhcp_setting.dhcpStart.value.replace(oldIp, newNet);
	//document.dhcp_setting.dhcpEnd.value = document.dhcp_setting.dhcpEnd.value.replace(oldIp, newNet);
	//document.dhcp_setting.dhcpGateway.value = document.dhcp_setting.lanIp.value;
}

	var dhcpstart = "<% getInfo("dhcpRangeStart"); %>";
	var dhcpend = "<% getInfo("dhcpRangeEnd"); %>";
	var staticip1 = "";
	var staticmac1 = "";
	var staticip2 = "";
	var staticmac2 = "";
	var staticip3 = "";
	var staticmac3 = "";
	//var dhcpstart = "111.111.111.12";
	//var dhcpend = "111.111.111.254";
	//var staticmac1 = "60:6C:66:BD:7B:03";
	//var staticip1 = "192.168.1.23";
	//var staticmac2 = "60:6C:66:BD:7B:04";
	//var staticip2 = "192.168.1.111";
	//var staticmac3 = "11:22:33:44:55:66";
	//var staticip3 = "10.10.10.12";
	//var staticmac3 = "";
	//var staticip3 = "";
function atoi(str, num)
{
	i = 1;
	if (num != 1) {
		while (i != num && str.length != 0) {
			if (str.charAt(0) == '.') {
				i++;
			}
			str = str.substring(1);
		}
		if (i != num)
			return -1;
	}

	for (i=0; i<str.length; i++) {
		if (str.charAt(i) == '.') {
			str = str.substring(0, i);
			break;
		}
	}
	if (str.length == 0)
		return -1;
	return parseInt(str, 10);
}

function checkRange(str, num, min, max)
{
	d = atoi(str, num);
	if (d > max || d < min)
		return false;
	return true;
}

function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function checkIpAddr(field, ismask)
{
	if (field.value == "") {
		alert("IP地址为空");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (isAllNum(field.value) == 0) {
		alert("请输入数字");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 0, 255)))
		{
			alert("IP地址错误");
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	else {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 1, 254)))
		{
			alert("IP 地址错误");
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
}
	
function check_value()
{
	if (!checkIpAddr(document.dhcp_setting.lanIp, false))
	{
		alert("IP 地址非法，请重新输入！");
		return false;
	}
	
	if(parseInt(document.dhcp_setting.startip4.value) >= parseInt(document.dhcp_setting.endip4.value))
	{
		alert("结束地址必须大于起始地址！");
		return false;
	}
	else
	{
		stip = new Array;
		stip = dhcpstart.split(".");
		edip = new Array;
		edip = dhcpend.split(".");
		var sturl= stip[0] + "." + stip[1] + "." + stip[2];
		document.dhcp_setting.starthidden.value = sturl + "." + document.dhcp_setting.startip4.value;
		var edurl= edip[0] + "." + edip[1] + "." + edip[2];
		document.dhcp_setting.endhidden.value = edurl + "." + document.dhcp_setting.endip4.value;
		if (!checkIpAddr(document.dhcp_setting.starthidden, false))
		return false;
		if (!checkIpAddr(document.dhcp_setting.endhidden, false))
		return false;
		
	}
	if(document.dhcp_setting.leasetime.value < 1 || document.dhcp_setting.leasetime.value > 168)
	{
		alert("租用时间最短为1小时,最长为一周！");
		return false;
	}
	else if(parseInt(document.dhcp_setting.leasetime.value) != document.dhcp_setting.leasetime.value)
	{
    alert("请输入整数！");
    return false;
	}
	else
	{
		document.dhcp_setting.realtime.value = (parseInt(document.dhcp_setting.leasetime.value))*3600;
	}
	if(document.dhcp_setting.macaddr.value != ""  && document.dhcp_setting.ipaddr.value != "")
	{
		if (!checkIpAddr(document.dhcp_setting.ipaddr, false))
		return false;
		var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
		if (!re.test(document.dhcp_setting.macaddr.value)) {
			var message = document.dhcp_setting.macaddr.value + "是非法MAC地址!";
			alert(message);
			return false;
		}		
		if((document.dhcp_setting.bdmac1.value == "") && (document.dhcp_setting.bdip1.value == ""))
		{
			document.dhcp_setting.static1.value = document.dhcp_setting.macaddr.value + ";" +document.dhcp_setting.ipaddr.value;
			//document.dhcp_setting.bdip1.value = document.dhcp_setting.ipaddr.value;
		}
		else if((document.dhcp_setting.bdmac2.value == "") && (document.dhcp_setting.bdip2.value == ""))
		{
			document.dhcp_setting.static2.value = document.dhcp_setting.macaddr.value + ";" +document.dhcp_setting.ipaddr.value;
			//document.dhcp_setting.bdmac2.value = document.dhcp_setting.macaddr.value;
			//document.dhcp_setting.bdip2.value = document.dhcp_setting.ipaddr.value;
		}
		else if ((document.dhcp_setting.bdmac3.value == "") && (document.dhcp_setting.bdip3.value == ""))
		{
			document.dhcp_setting.static3.value = document.dhcp_setting.macaddr.value + ";" +document.dhcp_setting.ipaddr.value;
			//document.dhcp_setting.bdmac3.value = document.dhcp_setting.macaddr.value;
			//document.dhcp_setting.bdip3.value = document.dhcp_setting.ipaddr.value;
		}
	}
	return true;
		
}
	
function dhcp_apply()
{
	if(check_value()==true)
	{
		document.dhcp_setting.submit(); 
		//alert("正在设置请稍等.....");
		Alert("正在保存设置，请稍候......", 20);
	}
}

$(function(){
	var currOpmode = "<%getHeInfo("opMode")%>";
    if(currOpmode==1){
		$('.ifdisabled').addClass('disabled');
		
		$('.disabled').click(function(event){  
	        return false;  
	    });
	}
	
})
</script>
</head>

<body>
<form method="post" name="dhcp_setting" id="dhcp_setting" action="/boafrm/dhcpbanding">
<div class="int">
	<h1>高级设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
                <li><a class="ifdisabled" href="WANConfig.asp">WAN设置</a></li>
				<li class="on"><a href="DHCPbanding.asp">局域网设置</a></li>
				<li><a class="ifdisabled" href="MACClone.asp">MAC地址克隆</a></li>
				<li><a class="ifdisabled" href="systemtime.asp">系统时间管理</a></li>
				<li><a href="sysupdate.asp">系统升级</a></li>
				<li><a href="restoredefault.asp">恢复出厂设置</a></li>
            </ul>
        </div>
        <div class="cfgright" style="padding-top:10px;padding-left: 12px;">
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
			  <tr>
				<td id="lIp" width="22%" align="left">IP 地址</td>
				<td><input class="inp2" name="lanIp" id="lanIp" maxlength=15 value="<%getInfo("ip-rom");%>" onFocus="recIpCfg()" onBlur="modDhcpCfg()"></td>
			  </tr>
              <tr>
                <td align="left">开关</td>
                <td>
					<div class="selectswitch" id="state">
						<em class="closestate"></em><span><strong id="switch"></strong></span><em class="openstate"></em>
						<input type="hidden" id="dhcphidden" name="dhcphidden" value="">
					</div>
				</td>
              </tr>
            </table>
			<div id="dhcpset">
				<table width="100%" border="0" cellspacing="0" cellpadding="0" style="padding-top:10px;">
				  <tr>
					<td id="lIp" width="22%" align="left">IP分配范围</td>
					<td>
						<input name="startip1" id="startip1" type="text"  value="<%getInfo("ip-mask");%>" readonly="readonly" style="background-color: #f2f2f1;width: 68px;padding: 5px;border: 0;text-align: center;">.
						<b><input name="startip4" id="startip4" maxlength="3" type="text"  style="width: 40px;padding: 5px;"></b>
						<input type="hidden" id="starthidden" name="starthidden" value="">
						到 <input name="endip1" id="endip1" type="text"  value="<%getInfo("ip-mask");%>" readonly="readonly" style="background-color: #f2f2f1;width: 68px;padding: 5px;border: 0;text-align: center;">.
						<b><input name="endip4" type="text" maxlength="3" style="width: 40px;padding: 5px;"></b>
						<input type="hidden" id="endhidden" name="endhidden" value="">
					</td>
				  </tr>
	              <tr>
	                <td align="left">租用时间</td>
	                <td>
						<b><input name="leasetime" id="leasetime" type="text" style="width: 40px;padding: 5px;"></b>&nbsp;小时
						<input type="hidden" id="realtime" name="realtime" value="">
					</td>
	              </tr>
	            </table>
	            <div class="bandingcfg" style="display: none">
					<strong>IP 与 MAC 地址绑定</strong><span onClick="javascript:addmacip();">新增绑定</span><b>显示当前DHCP设备列表</b>
				</div>
	            <div class="ipmaclist" style="display: none">
					<ul>
						<li><input name="macaddr" id="macaddr" type="text" placeholder="MAC 地址" title="MAC 地址"> 
						&gt; 
						<input name="ipaddr" id="ipaddr" type="text" placeholder="IP 地址" title="IP 地址"></li>
						<li id="bangding1"><em><input type="text" readonly="readonly" id="bdmac1" name="bdmac1" title="MAC 地址" value="" placeholder="MAC 地址"> 
						&gt; 
						<input type="text" readonly="readonly" id="bdip1" name="bdip1" title="IP 地址" value="" placeholder="IP 地址">
						</em><span onClick="javascript:delmacip(1);"></span><input type="hidden" id="static1" name="static1" value=""></li>
						<li id="bangding2"><em><input type="text" readonly="readonly" id="bdmac2" name="bdmac2" title="MAC 地址" value="" placeholder="MAC 地址"> 
						&gt; 
						<input type="text" readonly="readonly" id="bdip2" name="bdip2" title="IP 地址" value="" placeholder="IP 地址">
						</em><span onClick="javascript:delmacip(2);"></span><input type="hidden" id="static2" name="static2" value=""></li>
						<li id="bangding3"><em><input type="text" readonly="readonly" id="bdmac3" name="bdmac3" title="MAC 地址" value="" placeholder="MAC 地址"> 
						&gt; 
						<input type="text" readonly="readonly" id="bdip3" name="bdip3" title="IP 地址" value="" placeholder="IP 地址">
						</em><span onClick="javascript:delmacip(3);"></span><input type="hidden" id="static3" name="static3" value=""></li>
					</ul>
				</div>
		    </div>
			<div class="subpart">
                <ul>
                    <li><b id="subbtn">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
			

        </div>
    </div>
</div>
</form>
<script type="text/javascript">
    var dhcpenbl = '<%getHeInfo("dhcpServerEnable")%>';	
	var dhcpstart = "<% getInfo("dhcpRangeStart"); %>";
	var dhcpend = "<% getInfo("dhcpRangeEnd"); %>";
	var dhcpleasetime = '<% getInfo("dhcpLeaseTime"); %>';
	var staticmac1 = "";
	var staticip1 = "";
	var staticmac2 = "";
	var staticip2 = "";
	var staticmac3 = "";
	var staticip3 = "";
	//var dhcpenbl = 1;
	//var dhcpstart = "111.111.111.254";
	//var dhcpend = "111.111.111.254";
	//var dhcpleasetime = "86400"
	//var staticmac1 = "60:6C:66:BD:7B:03";
	//var staticip1 = "192.168.1.23";
	//var staticmac2 = "60:6C:66:BD:7B:04";
	//var staticip2 = "192.168.1.111";
	//var staticmac3 = "11:22:33:44:55:66";
	//var staticip3 = "10.10.10.12";
	//var staticmac3 = "";
	//var staticip3 = "";
	function addmacip()
	{
		if((document.dhcp_setting.bdmac1.value != "") && (document.dhcp_setting.bdip1.value != "") && (document.dhcp_setting.bdmac2.value != "") && (document.dhcp_setting.bdip2.value != "") && (document.dhcp_setting.bdmac3.value != "") && (document.dhcp_setting.bdip3.value != ""))
		{
			alert("最多只能添加三条绑定！");
			return;
		}
		else
		{
			document.getElementById("macaddr").disabled = "";
			document.getElementById("ipaddr").disabled = "";
		}
		
	}
	function delmacip(id)
	{
		if(id == '1')
		{
			document.getElementById("bangding1").style.display = "none";
			document.dhcp_setting.bdmac1.value ="";
			document.dhcp_setting.bdip1.value ="";
			document.dhcp_setting.static1.value = "";
		}
		else if(id == '2')
		{
			document.getElementById("bangding2").style.display = "none";
			document.dhcp_setting.bdmac2.value ="";
			document.dhcp_setting.bdip2.value ="";
			document.dhcp_setting.static2.value = "";
		}
		else
		{
			document.getElementById("bangding3").style.display = "none";
			document.dhcp_setting.bdmac3.value ="";
			document.dhcp_setting.bdip3.value ="";
			document.dhcp_setting.static3.value = "";
		}
	}
	function call_edip(max_length) 
	{
			var size = document.dhcp_setting.endip1.size; 
			
			if((document.dhcp_setting.endip1.value == null ) || (document.dhcp_setting.endip1.value == "" )) document.dhcp_setting.endip1.size = size;
			if((document.dhcp_setting.endip1.value.length >= size) && (document.dhcp_setting.endip1.value.length <= max_length)) document.dhcp_setting.endip1.size = document.dhcp_setting.endip1.value.length + 1;
			else document.dhcp_setting.endip1.size = size;
	}
	function call_stip(max_length) 
	{
		
			var size = document.dhcp_setting.startip1.size; 
			
			if((document.dhcp_setting.startip1.value == null ) || (document.dhcp_setting.startip1.value == "" )) document.dhcp_setting.startip1.size = size;
			if((document.dhcp_setting.startip1.value.length >= size) && (document.dhcp_setting.startip1.value.length <= max_length)) document.dhcp_setting.startip1.size = document.dhcp_setting.startip1.value.length + 1;
			else document.dhcp_setting.startip1.size = size;
			var size = document.dhcp_setting.endip1.size; 
			
			if((document.dhcp_setting.endip1.value == null ) || (document.dhcp_setting.endip1.value == "" )) document.dhcp_setting.endip1.size = size;
			if((document.dhcp_setting.endip1.value.length >= size) && (document.dhcp_setting.endip1.value.length <= max_length)) document.dhcp_setting.endip1.size = document.dhcp_setting.endip1.value.length + 1;
			else document.dhcp_setting.endip1.size = size;
	}
	function $$$$$(_sId)
	{
 		return document.getElementById(_sId);
 	}
	function hide(_sId)
	{
		$$$$$(_sId).style.display = $$$$$(_sId).style.display == "none" ? "" : "none";
	 }
	function pick(v,targetid,abc) 
	{
		document.getElementById(targetid).value=v;
		hide(abc);
	}
	$(document).ready(function()
	{
	  var switchcode=dhcpenbl;
	  document.dhcp_setting.dhcphidden.value = switchcode;
		if(dhcpenbl==0)
		{
			$("#state").addClass("switchoff");
			$("#dhcpset").css("display","none");
			$(".macinfo").css("display","none");
		}
		else
		{
			$("#state").removeClass("switchoff");
			$("#dhcpset").css("display","");
			$(".macinfo").css("display","");
		}
		
		stip = new Array;
		stip = dhcpstart.split(".");
		edip = new Array;
		edip = dhcpend.split(".");
		var sturl= stip[0] + "." + stip[1] + "." + stip[2];
		document.dhcp_setting.startip1.value = sturl;
		document.dhcp_setting.startip4.value = stip[3];
		call_stip(20);
		var edurl= edip[0] + "." + edip[1] + "." + edip[2];
		document.dhcp_setting.endip1.value = edurl;
		document.dhcp_setting.endip4.value = edip[3];
		call_edip(20);
		
		//document.dhcp_setting.leasetime.value = dhcpleasetime/3600;
		document.dhcp_setting.leasetime.value = dhcpleasetime/60;
		document.getElementById("macaddr").disabled = "disabled";
		document.getElementById("ipaddr").disabled = "disabled";
		//document.dhcp_setting.macaddr.disabled = disabled;
		//document.dhcp_setting.ipaddr.disabled = disabled;
		if((staticmac1 != "") && (staticip1 != ""))
		{
			document.dhcp_setting.bdmac1.value =staticmac1;
			document.dhcp_setting.bdip1.value =staticip1;
			document.dhcp_setting.static1.value = staticmac1 + ";" + staticip1;
			document.getElementById("bangding1").style.display = "hidden";
		}
		else
		{
			document.getElementById("bangding1").style.display = "none";
		}
		
		if((staticmac2 != "") && (staticip2 != ""))
		{
			document.dhcp_setting.bdmac2.value =staticmac2;
			document.dhcp_setting.bdip2.value =staticip2;
			document.dhcp_setting.static2.value = staticmac2 + ";" + staticip2;
			document.getElementById("bangding2").style.display = "hidden";
		}
		else
		{
			document.getElementById("bangding2").style.display = "none";
		}
		if((staticmac3 != "") && (staticip3 != ""))
		{
			document.dhcp_setting.bdmac3.value =staticmac3;
			document.dhcp_setting.bdip3.value =staticip3;
			document.dhcp_setting.static3.value = staticmac3 + ";" + staticip3;
			document.getElementById("bangding3").style.display = "hidden";
		}
		else
		{
			document.getElementById("bangding3").style.display = "none";
		}
		
		$("#switch").click(function(){
			if(switchcode==1){
				 $("#state").addClass("switchoff");
				 switchcode=0;
				 document.dhcp_setting.dhcphidden.value =switchcode;
				 
				 $("#dhcpset").css("display","none");
				 $(".macinfo").css("display","none");
				}else{
				$("#state").removeClass("switchoff");
				$("#dhcpset").css("display","");
				$(".macinfo").css("display","");
				switchcode=1;
				document.dhcp_setting.dhcphidden.value =switchcode;
				}
		});
		
		
		$("#subbtn").click(function(){
			if(check_value()==false)
				return false;
				
		Alert("正在保存设置，请稍候", 10);	
		setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面', 20)", 10100);	
      $.ajax({
      	type: "post",
        url: "/boafrm/dhcpbanding",     
        data: $("#dhcp_setting").serialize(),    
        success: function(data){
        	//alert("保存成功！");
        },
        error: function(data) {
        	//alert("保存失败！\n" + data);
        }
      })
    });
    
	});


</script>
</body>
</html>
