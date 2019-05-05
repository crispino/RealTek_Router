<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta name="KEYWords" Content="wifisetting1pageFlag">
<meta http-equiv="Pragma" CONTENT="no-cache">
<meta http-equiv="Cache-Control" CONTENT="no-cache">
<meta http-equiv="Expires" CONTENT="-1">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<style type="text/css">
<!--
body{ color:#676767;}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:410px; width:670px; overflow:hidden; background:#fcfcfa url(images/fullbg.gif) repeat-x; padding:5px 15px;}
.int h1{ font-size:16px; line-height:50px;	color: #cccccc; border-bottom:1px #d3d3d3 solid;}
.wifiset{ padding:30px; }
.wifiset td{ height:36px;}
.wifiset td span{ padding-left:70px; font-size:14px;}
.wifiset td b{ font-weight:normal; padding-right:22px; color:#999; line-height:26px; display:block; height:26px; float:left; background:url(images/help.png) no-repeat right center;}
.wifiset td i{ color:#999; font-style:normal;}
.inp1{ height:26px; border:1px #d2d2d2 solid; width:230px;font-family:"微软雅黑"; color:#666; padding:0 10px;}
.inp2{ height:26px; border:1px #d2d2d2 solid; width:37px;font-family:"微软雅黑"; color:#666; padding:0 10px;}
.inp3{ height:26px; border:1px #d2d2d2 solid; width:163px;font-family:"微软雅黑"; color:#666; padding:0 10px;}
.checkinp{height:26px; width:26px; padding:0; border:none; background:#f5f5f5;}
.selectinp{height:28px; border:1px #d2d2d2 solid; width:230px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 10px; background:#fff url(images/dropd.png) no-repeat right center;}
.selectinp_custom{height:28px; border:1px #d2d2d2 solid; width:252px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 10px; background:#fff url(images/dropd.png) no-repeat right center;}
select {
    appearance: none;
    -moz-appearance: none;
    -webkit-appearance: none;
}
.bm {margin-top:30px;position:absolute;}
.ganyin{ position:absolute; height:30px;cursor:pointer; border:1px #333 solid; z-index:100001; background:#0099CC;filter:alpha(opacity=0);-moz-opacity:0;-khtml-opacity: 0;opacity: 0;}
.bm ul{ padding:1px 0 0 0; border-bottom:1px #eee solid; border-left:1px #eee solid; border-right:1px #eee solid; }
.bm li{ color:#0781d9; line-height:30px; padding-left:10px; height:30px; background:#fff; font-size:12px; text-align:left; padding-right:12px; display:block;}
.bm .nowon{ color:#adadae;}
.bm li:hover{ background-color:#b2e9fd;cursor:pointer;}


.selectswitch{ height:29px;}
.selectswitch em{ float:left; height:20px; width:20px; display:block; margin:4px; padding:0;}
.selectswitch .closestate{ background:url(images/wifistateicon.png) no-repeat;}
.selectswitch .openstate{background:url(images/wifistateicon.png) no-repeat 0 -40px;}
.wifiset td .selectswitch span{ float:left; padding-left:0px; background:url(images/selbg.png) no-repeat; width:55px; height:29px; display:block;}
.selectswitch strong{ display:block; margin:3px; height:23px; width:49px; background:url(images/select.png) no-repeat right center; cursor:pointer;}


.switchoff{}
.switchoff .closestate{ background-position:0 -20px;}
.switchoff .openstate{ background-position:0 -60px;}
.switchoff  strong{ background-position:left center;}


.subpart{ height:48px; padding-left:155px;}
.subpart ul{}
.subpart li{ float:left; padding-right:10px;}
.subpart b{ display:block; height:48px; cursor:pointer; width:170px; font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:48px;}
.subpart b:hover{ background:#56bbcb;}
.subpart .cancel{ width:170px; background:#99e4f0; }
.c2{ border-top:1px #e6e6e6 solid; line-height:30px; position:relative; bottom:65px}
.Txttype {
font-size:20px;
 line-height:50px; 
	color: #000000;
	
}

-->
</style>
<script type="text/javascript">

var WIFIAuthMode= '<%getHeInfo("WIFIAuthMode", "wlan2");%>';
var wifi_off= '<%getHeInfo("wifi_off", "wlan2")%>';
var Key1Type= '<%getHeInfo("keyType", "wlan2")%>';
var EncrypType='<%getHeInfo("EncrypType", "wlan2");%>';	
var RekeyInterval='<%getHeInfo("groupRekeyTime", "wlan2");%>';
var HideSSID= '<%getHeInfo("hiddenSSID", "wlan2")%>';
var wpsenable  = '<%getHeInfo("wscDisable", "wlan2")%>';
var IEEE8021X  = '<%getHeInfo("enable1X", "wlan2")%>';
var WPAPSK1  = '<%getHeInfo("pskValue", "wlan2")%>';
var Key1Str1  = '';
var ssid1 = '<%getHeInfo("SSID", "wlan2")%>';
var txpower = '<%getHeInfo("RFPower", "wlan2")%>';
var adminpasswd = '<%getHeInfo("admin_passwd")%>';
var wlan_num = <% getHeInfo("wlan_num"); %>;
var click_num;
var wifi1_off='<%getHeInfo("wifi_off", "wlan1")%>';
var wifi2_off='<%getHeInfo("wifi_off", "wlan2")%>';
function $$$$$(_sId){
 return document.getElementById(_sId);
 }
 
function hide(_sId){
	$$$$$(_sId).style.display = $$$$$(_sId).style.display == "none" ? "" : "none";
 }
 
 
function isHiddenField(ishidden,hid_id){

    if(ishidden == 1){
	      document.getElementById(hid_id).style.display = "none";

    }else{
	      document.getElementById(hid_id).style.display = "";
    }
} 
 
function show_sub(v) {     
	if(v == 'OPEN') {		
		isHiddenField(1,"hepwd");
	} else {
		isHiddenField(0,"hepwd");
	}
}  


function pick2(v,targetid,abc) {
      
  document.getElementById(targetid).value=v;
  hide(abc);  
}

function isCloseWIFI(close){
	
    if(close == 1){
	       isHiddenField(1,"networkName");
	       isHiddenField(1,"authType");
	       isHiddenField(1,"hepwd");
//	       isHiddenField(1,"managepwd");
	       $("div.c2").css("display","none");
	       isHiddenField(1,"txpower");
	  }else{
	       isHiddenField(0,"networkName");
	       isHiddenField(0,"authType");
//	       isHiddenField(0,"managepwd");
	       $("div.c2").css("display","block");
	       isHiddenField(0,"txpower");
	       if(document.getElementById("security_mode").value == "OPEN"){
	           isHiddenField(1,"hepwd");	
	       }else{
	           isHiddenField(0,"hepwd");;
	       }
	  }
}

$(document).ready(function(){
	
	if(<% getIndex("opMode"); %>!=0){
		setTitle();
	}
	
		//TxPower
	var local_txpower = 1;
	if(txpower >= 80 && txpower <=100) {
		local_txpower = 0
	} else if(txpower <=60) {
		local_txpower = 2
	} else {
		local_txpower = 1
	}
	document.getElementById("wifi_strength").value = local_txpower;
	
	click_num = 0;
	//var $password = $('#passphrase');
	//var	$passwordInput = $('<input type="text" name="' + $password.attr('name') + '" class="' + $password.attr('class') + '" />');

	document.getElementById("WEP1Select").value= Key1Type;
	var RekeyIntervalArray = RekeyInterval.split(";");
	document.getElementById("keyRenewalInterval").value= RekeyIntervalArray[0];

  document.wireless_setting.wep_key_1.value = Key1Str1;
	document.wireless_setting.passphrase.value = WPAPSK1;
//	document.wireless_setting.admpass.value = adminpasswd;
	
	//var str = new Array();
	//str = ssid1.split("_"); 
	//document.wireless_setting.mssid_prefix.value = "CMCC" + "_";	
	document.wireless_setting.mssid.value = ssid1;
	
	/*
	for(i=1; i<str.length;++i){
		if(i<(str.length-1))
			document.wireless_setting.mssid.value = (document.wireless_setting.mssid.value + str[i] + "_");
		else
			document.wireless_setting.mssid.value = (document.wireless_setting.mssid.value + str[i] );
	}*/
	
	var WIFIAuthModeArray = WIFIAuthMode.split(";");

	
	
	
	if(WIFIAuthModeArray[0] == "OPEN"){
		  document.getElementById("security_mode").value= "OPEN";	
		  isHiddenField(1,"hepwd");


		  document.wireless_setting.passphrase.value = Key1Str1;
	}else 
	{
		  document.getElementById("security_mode").value="WPAPSKWPA2PSK";		
		  isHiddenField(0,"hepwd");
//	      isHiddenField(0,"use_wifipwd");
//	      isHiddenField(0,"usewifitext");
		  document.wireless_setting.passphrase.value = WPAPSK1;
	}
  
	//wifi off to on
	if(wifi1_off == 1 && wifi2_off ==1){
	    $("#state").addClass("switchoff");
	    document.wireless_setting.wifihiddenButton.value= 0; 
      isCloseWIFI(1);
	    
	}else{
		  $("#state").removeClass("switchoff");
		  document.wireless_setting.wifihiddenButton.value= 1; 
      isCloseWIFI(0);
	}
	
	if(EncrypType == "NONE")
	    document.getElementById("cipher").value= 0;
	else 
		  document.getElementById("cipher").value= EncrypType;    

	var HideSSIDArray = HideSSID.split(";");
	if(HideSSIDArray[0] == "0"){
      document.getElementById("hssid").checked = false;
	}else{
      document.getElementById("hssid").checked = true;
	}

	
	$("#switch").click(function(){
		click_num++;
		if(click_num%2 == 1){
			//not necessarily turn to open
		  if( wifi1_off == 1 && wifi2_off == 1){
			  $("#state").removeClass("switchoff");
			    document.wireless_setting.wifihiddenButton.value = 1;

			    isCloseWIFI(0);
			  }else{
			  $("#state").addClass("switchoff");
			    document.wireless_setting.wifihiddenButton.value = 0;

			    isCloseWIFI(1);
			 }
		}else{
		  if(wifi1_off == 1 && wifi2_off == 1){
			  $("#state").addClass("switchoff");
			    document.wireless_setting.wifihiddenButton.value = 0;

			    isCloseWIFI(1);
			  }else{
			  $("#state").removeClass("switchoff");
			    document.wireless_setting.wifihiddenButton.value = 1;

			    isCloseWIFI(0);
			 }
			
		}
	})
 
	$('#show_password').click(function(){
		if(this.checked){
			ps();
		}else{
			txt();
		}
	});
	
//	$('#use_wifipwd').click(function(){
//		if(this.checked){
//			document.getElementById("admpass").value = document.getElementById("passphrase").value;
//		}else
//			document.getElementById("admpass").value = adminpasswd;
//	});	

});

function checkInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if ( str.charAt(i) == '\r' || str.charAt(i) == '\n'){
				return false;
		}else
	        continue;
	}
    return true;
}

function checkHex(str){
	var len = str.length;

	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') ){
				continue;
		}else
	        return false;
	}
    return true;
}

function check_value(){
   
	var securitymode;
	var ssidName;
	
	securitymode = document.wireless_setting.security_mode.value;
	ssidName = document.wireless_setting.mssid.value;
	
	if(document.wireless_setting.wifihiddenButton.value == 0){
	    return true;
	}
	
	if(document.wireless_setting.mssid.value == "")
	{
		alert("请输入完整的网络名称");
		return;
	}

	if(wlan_num==1){
		if(ssidName.length < 4 || ssidName.length > 26 )
		{
	     	 alert('输入的网络名称长度需为4-26个英文字母或数字');
			 return false;	
		}
	}else if(wlan_num==2){
		if(ssidName.length < 4 || ssidName.length > 21 )
		{
	     	 alert('输入的网络名称长度需为4-21个英文字母或数字');
			 return false;	
		}
	}
	
	if (securitymode == "OPEN")
	{
		if (wpsenable != "0") 
			alert('此安全类型没有密码!');
	}else {
		var keyvalue = document.wireless_setting.passphrase.value;
    document.wireless_setting.wep_key_1.value = Key1Str1;
    
		if (keyvalue.length == 0){
			alert('请输入无线密码');
			return false;
		}

		if (keyvalue.length < 8 || keyvalue.length > 64 ){
			alert('密码需输入8到63个字母或者数字，或者64个十六进制字符');
			return false;
		}

		if (keyvalue.length >= 64 && checkHex(keyvalue) == false) {
			alert('密码需输入8到63个字母或者数字，或者64个十六进制字符密码');
			return false;
		}
		
		if(checkInjection(document.wireless_setting.passphrase.value) == false){
			alert('密码包含非法字符，请重新输入');
			return false;
		}
    
   
	}
	
	if(document.getElementById("hssid").checked == true)
	    document.wireless_setting.hssid.value = "0"; 
	else
		  document.wireless_setting.hssid.value = ""; 

 		        

	return true;
}

function afterSubmit(){

  alert("设置完成");
  parent.TINY.box.hide();	
  

  //window.location.reload();
  setTimeout("javascript:location.href='/customer/main.html'", 1000);
}

function security_apply(){
	
	if (check_value() == true)
	{
		//document.wireless_setting.mssid_0.value = "CMCC_" + document.wireless_setting.mssid.value;
		document.wireless_setting.mssid_0.value = document.wireless_setting.mssid.value;
		
	
//	console.log($("#wireless_setting").serialize());
      $.ajax({
      	type: "post",
        url: "/boafrm/wirelessQuick",     
        data: $("#wireless_setting").serialize(),    
        success: function(data){
        },
        error: function(data) {
        }
      })
	    
	    setTimeout("javascript:location.href='/herouter/wifistatus.asp'", 1000);
		
	}
}
function ps(){
	if (document.getElementById("passphrase").type=="password"){
		var passwd = document.getElementById("passphrase").value;
		var passwd1 = "";		
		
		for(i=0; i<passwd.length;++i){			
			if(passwd[i] == '"'){				
				passwd1 = passwd1 + '&quot;';							
			}else
				passwd1 = passwd1 + passwd[i];
		}	
		
		document.getElementById("passortext").innerHTML="<input class=\"inp1\" type=\"text\" name=\"passphrase\"  id=\"passphrase\" value=\""+passwd1+"\">";
	}
}

function txt()
{
	if (document.getElementById("passphrase").type=="text"){
		var passwd = document.getElementById("passphrase").value;
		var passwd1 = "";

		for(i=0; i<passwd.length;++i){			
			if(passwd[i] == '"'){				
				passwd1 = passwd1 + '&quot;';							
			}else
				passwd1 = passwd1 + passwd[i];
		}		
		document.getElementById("passortext").innerHTML="<input class=\"inp1\" type=\"password\" name=\"passphrase\"  id=\"passphrase\" value=\""+passwd1+"\">";
	}
}

function cancel(){
	parent.TINY.box.hide();
}
function Back(){
	parent.TINY.box.show({iframe:'QuickSetUp.asp',boxid:'frameless',width:700,height:420,fixed:false,maskopacity:40});
}

function setTitle()
{
	$("#step1").css({"display":"none"});
	$("#step2").text("(第一步)无线设置-->");
	$("#step3").text("(第二步)完成");
}
</script>
</head>

<body>
<form method="post" name="wireless_setting" id="wireless_setting" action="/boafrm/wirelessQuick">
<div class="int">
	<h1 align="center"><span id="step1">(第一步)互联网设置—></span>
		<span class="Txttype" id="step2">(第二步)无线设置<em>--></em></span>
		<span id="step3">(第三步)完成</span>
	</h1>
	<div class="wifiset">
		<table width="100%" border="0" cellspacing="0" cellpadding="0">
		  <tr>
			<td width="27%"><span>无线网络开关</span></td>
			<td width="42%">
				<div class="selectswitch" id="state">
					<em class="closestate"></em><span><strong id="switch"></strong></span><em class="openstate"></em>
				</div>
				<input type="hidden" id="wifihiddenButton" name="wifihiddenButton" value="1">
			</td>
			<td width="5%">&nbsp;</td>
			<td width="26%">&nbsp;</td>
		  </tr>
		  <tr id="networkName">
			<td><span>网络名称</span></td>
			<td>
			  <!--<input class="inp2" disabled="disabled" type="text" maxlength="5" id="mssid_prefix" name="mssid_prefix" value="" style=" width:42px;"/> -->
			  <input class="inp1" type="text" maxlength="28" id="mssid" name="mssid" value="" onkeyup="value=value.replace(/[^\w\.\-\/]/ig,'')"/>
			  </td>
			<td align="center"><input class="checkinp" type="checkbox" id="hssid" name="hssid"/></td>
			<td><i>隐藏网络</i></td>
		  </tr>
		  <tr id="authType">
			<td><span>安全类型</span></td>
			<td>
			<!--<div class="fix" style=" z-index:999;">
				<div class="ganyin" onclick="hide('test1')" style=" width:250px;"></div>
					<div id="test1" style="display: none; " class="bm">
					<ul style=" width:250px;">
						<li id="a11" onclick="pick('不加密','security_mode','test1')" >不加密</li>
						<li id="a15" onclick="pick('加密','security_mode','test1')">加密</li>
					</ul>
				</div>
			</div>
			<input type="text" class="selectinp" id="security_mode" name="security_mode" value="" />-->
			<select class="selectinp_custom" id="security_mode" name="security_mode" onchange="show_sub(this.options[this.options.selectedIndex].value)">
				<option value='OPEN'>不加密</option>
				<option value='WPAPSKWPA2PSK'>加密</option>
			</select>
			</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		  </tr>
		  <tr id="hepwd">
			<td><span>无线密码</span></td>
			<td id="passortext"><input class="inp1" type="text" name="passphrase" id="passphrase"/></td>
			<td align="center"><input class="checkinp" type="checkbox" id="show_password" name="show_password" value="" checked="checked"/></td>
			<td><i>显示密码</i></td>
		  </tr>
		  
		  <!--<tr id="managepwd">
			<td><span>管理员密码</span></td>
			<td id="admtext"><input class="inp1" type="text" name="admpass" id="admpass" value=''/></td>
			<td align="center"><input class="checkinp" type="checkbox" id="use_wifipwd" name="use_wifipwd" value="" /></td>
			<td id="usewifitext"><i>与Wi-Fi密码一致</i></td> 
		 </tr>-->
		 <tr id="txpower">
			<td><span>信号强度</span></td>
			<td>
			  <select class="selectinp_custom" id="wifi_strength" name="wifi_strength">
				<option value='0'>超强（适用于三居室）</option>
				<option value='1'>标准（适用于二居室）</option>
				<option value='2'>节能（适用于一居室）</option>
			  </select>
			</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			</tr>
		  <tr>
		  	<td><input class="inp1" type="text" maxlength="32" id="mssid_0" name="mssid_0" value="" style="display: none;/></td>
		  </tr>
		  <div style="style:display:none">
		   <tr style="display: none; ">
			   <td><input class="inp1" name="admuser" value='admin' style="display: none;"/></td>		   	
			   <td><input name="wep_key_1" id="wep_key_1" value=""  style="display: none;"/></td>
			   <td><input name="bssid_num" id="bssid_num" value="2" style="display: none;"/></td>
			   <td><input name="ssidIndex" id="ssidIndex" value="0"  style="display: none;"/></td>
			   <td><input name="wep_default_key" id="wep_default_key" value="1"  style="display: none;"/></td>
			   <td><input name="WEP1Select" id="WEP1Select" value="1"  style="display: none;"/></td>
			   <td><input name="cipher" id="cipher" value="0"  style="display: none;"/></td>
			   <td><input name="keyRenewalInterval" id="keyRenewalInterval" value="0"  style="display: none;"/></td>
		   </tr>
	    </div>
		</table>
	</div>
	<div class="subpart">
		<ul>
			<li><b onclick="javascript:security_apply();">保存</b></li>
			<li><b class="cancel" onclick="javascript:cancel();">取消</b></li>
		</ul>
	</div>
	<div class="c2"></div>
</div>
</form>
</body>
</html>
