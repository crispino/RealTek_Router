<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Pragma" CONTENT="no-cache">
<meta http-equiv="Cache-Control" CONTENT="no-cache">
<meta http-equiv="Expires" CONTENT="0">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<title>无线网络设置1</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>

<script type="text/javascript">

var WIFIAuthMode="<%getHeInfo("WIFIAuthMode", "wlan2");%>";
var wifi_off="<%getHeInfo("wifi_off", "wlan2")%>";
var DefaultKeyID = "<%getHeInfo("default_keyid", "wlan2")%>";
var EncrypType="<%getHeInfo("cipher_type", "wlan2");%>";
var EncrypType_wpa2="<%getHeInfo("wpa2_cipher_type", "wlan2");%>";
<!--var RekeyInterval="";unused-->
var HideSSID='<%getHeInfo("hiddenSSID", "wlan2")%>';
var WPAPSK1  = '<%getHeInfo("pskValue", "wlan2")%>';
var ssid1 = '<%getHeInfo("SSID", "wlan2")%>';
var txpower = '<%getHeInfo("RFPower", "wlan2")%>';
var ht_bw = "<%getHeInfo("bandwidth", "wlan2")%>";
var ht_2040_coexit = "<%getHeInfo("ht_2040_coexist", "wlan2")%>";
var ChannelSeleted = "<%getHeInfo("channel_selected", "wlan2")%>";
var click_num;

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
 
function get_hidden_key(keylen){
	var str5 ="*****";
	var str10 ="**********";
	var str13 ="*************";
	var str26 ="**************************";
	if(keylen == 5)
		return str5;
	else if(keylen == 10)
		return str10;
	else if(keylen == 13)
		return str13;
	else if(keylen == 26)
		return str26;
} 
function pick(v,targetid,abc) {
	
	document.getElementById(targetid).value=v;
	hide(abc);
	if(v == "不加密"){
      isHiddenField(1,"hepwd");
      document.wireless_setting.wep_key_1.value = Key1Str1;
	    document.wireless_setting.passphrase.value = WPAPSK1;
	}
	else
	{   
		  if(v == "OPEN"){
		      document.wireless_setting.passphrase.value = Key1Str1;	
		  }else{
		  	  document.wireless_setting.passphrase.value = WPAPSK1;	
		  }
      isHiddenField(0,"hepwd");
	}
}

function pick2(v,targetid,abc) {
    
  document.getElementById(targetid).value=v;
  hide(abc);
}

function isCloseWIFI(close){
	
    if(close == 1){	
			isHiddenField(1,"signalChannel");
			isHiddenField(1,"networkName");
			isHiddenField(1,"authType");
			isHiddenField(1,"hepwd");
			isHiddenField(1,"txpower");
			isHiddenField(1,"wepKey");
			isHiddenField(1,"algorithms");		   
 
	  }else if(close == 0){
			isHiddenField(0,"signalChannel");
			isHiddenField(0,"networkName");
			isHiddenField(0,"authType");
			isHiddenField(0,"txpower");

			var auth_mode = document.getElementById("security_mode").value;
			if(auth_mode == 'OPEN') {
				isHiddenField(1,"hepwd");
				isHiddenField(1,"wepKey");
				isHiddenField(1,"algorithms");
			} else if (auth_mode == 'WEP') {
				isHiddenField(1,"hepwd");
				isHiddenField(1,"algorithms");
				isHiddenField(0,"wepKey");		
				
			} else {		
				isHiddenField(0,"hepwd");
				isHiddenField(0,"algorithms");
				isHiddenField(1,"wepKey");
			}
  
	}
}

$(document).ready(function(){
	
	click_num = 0;
	
	//SSID1
	/*
	var str = new Array();
	str = ssid1.split("_"); 
	if (str.length > 1) {
		var prefix = str[0] + "_";
		document.wireless_setting.mssid_prefix.value = prefix;
		document.wireless_setting.mssid.value = ssid1.replace(prefix, "");
	} else {
		document.wireless_setting.mssid_prefix.value = "CMCC_";
		document.wireless_setting.mssid.value = "";
	}
	*/
	document.wireless_setting.mssid.value = ssid1;

	var WIFIAuthModeArray = WIFIAuthMode.split(";");
	
	//信道和频宽
	document.wireless_setting.channel.value = ChannelSeleted;
	
	if (ht_bw == 0 && ht_2040_coexit != 1) { //20M
		document.wireless_setting.bandwith.value = 0
	} else if(ht_bw == 0 && ht_2040_coexit == 1) { //20/40M
		document.wireless_setting.bandwith.value = 1
	} else { // 40M
		document.wireless_setting.bandwith.value = 2
	}

	//AuthMode
	var EncryptTypeArray = EncrypType.split(";");
	var EncryptTypeArray_wpa2=EncrypType_wpa2.split(";");
	var encryptType = EncryptTypeArray[0];
	var encryptType_wpa2=EncryptTypeArray_wpa2[0];
	var auth_mode = WIFIAuthModeArray[0];
	if(auth_mode == 'OPEN' ) {
		document.getElementById("security_mode").value= "OPEN";
		isHiddenField(1,"hepwd");
		isHiddenField(1,"wepKey");
		isHiddenField(1,"algorithms");
	} else if (auth_mode == 'WEP') {
		document.getElementById("security_mode").value= "WEP";
		isHiddenField(1,"hepwd");
		isHiddenField(1,"algorithms");
		isHiddenField(0,"wepKey");
		
		//WEP设置
		var Key1Type,  Key1Str1;
		var key_length="<%getHeInfo("key_string_length", "wlan2")%>";
		/*
		if (DefaultKeyID == '1') {
			Key1Type = "<%getHeInfo("keyType", "wlan2")%>";
			Key1Str1 = get_hidden_key(key_length);
		}  else if (DefaultKeyID == '2') {
			Key1Type = "<%getHeInfo("key_type", "wlan2")%>";
			Key1Str1 = get_hidden_key(key_length);
		} else if (DefaultKeyID == '3') {
			Key1Type = "<%getHeInfo("key_type", "wlan2")%>";
			Key1Str1 = get_hidden_key(key_length);
		} else {
			Key1Type = "<%getHeInfo("key_type", "wlan2")%>";
			Key1Str1 = get_hidden_key(key_length);
		}
		*/
		Key1Type = "<%getHeInfo("keyType", "wlan2")%>";
		Key1Str1 = get_hidden_key(key_length);
		$("#wep_key1").val(Key1Str1);
		var defaultKeyArray = new Array();
		defaultKeyArray = DefaultKeyID.split(";");
		document.wireless_setting.wep_default_key.value = defaultKeyArray[0];
		
		defaultKeyArray = Key1Type.split(";");
		document.wireless_setting.wep_Select.value = (defaultKeyArray[0] == 0)?1:0;
		
	} else {
		if (auth_mode != 'WPAPSK' && auth_mode != 'WPA2PSK') auth_mode = 'WPAPSKWPA2PSK';
		document.getElementById("security_mode").value= auth_mode;
		isHiddenField(0,"hepwd");
		isHiddenField(0,"algorithms");
		isHiddenField(1,"wepKey");
	}
    
	//TxPower
	var local_txpower = 1;
	if ((txpower == 0) || ((txpower>60)&&(txpower<=100))) {
		local_txpower = 0;
	} else if((txpower <= 60)&& (txpower>40)){
		local_txpower = 1;
	} else if((txpower <= 40)&& (txpower>0)) {
		local_txpower = 2
	} else {
		local_txpower = 0
	}
	document.getElementById("wifi_strength").value = local_txpower;
	
	//wifi off to on
	if (wifi_off == 1) {
	    $("#state").addClass("switchoff");
	    document.wireless_setting.wifihiddenButton.value= 0; 
		isCloseWIFI(1);	    
	}else{
		$("#state").removeClass("switchoff");
		document.wireless_setting.wifihiddenButton.value= 1; 
     	isCloseWIFI(0);
	}
	
	//加密算法
	if(auth_mode == "WPAPSK"){
		if (encryptType == "TKIP") {
			document.wireless_setting.cipher[0].checked = true;
		} else if  (encryptType == "AES") {
			document.wireless_setting.cipher[1].checked = true;
		} else {
			document.wireless_setting.cipher[2].checked = true;
		}
	}
	if(auth_mode == "WPA2PSK"){
		if (encryptType_wpa2 == "TKIP") {
			document.wireless_setting.cipher[0].checked = true;
		} else if  (encryptType_wpa2 == "AES") {
			document.wireless_setting.cipher[1].checked = true;
		} else {
			document.wireless_setting.cipher[2].checked = true;
		}
	}
	if(auth_mode == "WPAPSKWPA2PSK"){
		if (encryptType_wpa2 == "TKIP") {
			document.wireless_setting.cipher[0].checked = true;
		} else if  (encryptType_wpa2 == "AES") {
			document.wireless_setting.cipher[1].checked = true;
		} else {
			document.wireless_setting.cipher[2].checked = true;
		}
	}

	//隐藏SSID
	var HideSSIDArray = HideSSID.split(";");
	if(HideSSIDArray[0] == "0"){
      document.getElementById("hssid").checked = false;
	} else {
      document.getElementById("hssid").checked = true;
	}
 
	$("#switch").click(function(){
		click_num++;
		if(click_num%2 == 1){
			//turn to open
		  if(wifi_off==1){
			  $("#state").removeClass("switchoff");
			    document.wireless_setting.wifihiddenButton.value = 1;					
			    isCloseWIFI(0);
			  }else{
			  $("#state").addClass("switchoff");
			    document.wireless_setting.wifihiddenButton.value = 0;
			    isCloseWIFI(1);
			    
			 }
		}else{
		  if(wifi_off==1){
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
	document.wireless_setting.show_password.checked=true;
	document.wireless_setting.passphrase.value = WPAPSK1;
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
	if ( str == "*****" ||
       str == "**********" ||
       str == "*************" ||
       str == "**************************" )
       return true;
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
   
	var securitymode = document.wireless_setting.security_mode.value;
	var ssidName = document.wireless_setting.mssid.value;	
	ssidName = ssidName.replace(/[^\w\.\-\/]/ig,'');
	document.wireless_setting.mssid.value = ssidName;	
	if(ssidName.length < 4 || ssidName.length > 32 )
	{
		alert('输入的网络名称长度需为4-32个英文字母或数字');
		return false;	
	}
	
	//信道设置
	if(document.getElementById("bandwith").value == "0")//20M
	{
		document.getElementById("n_bandwidth").value="0";
		document.getElementById("n_2040_coexit").value="0";
	}
	else if(document.getElementById("bandwith").value == "1")//20/40M
	{
		document.getElementById("n_bandwidth").value="1";
		document.getElementById("n_2040_coexit").value="1";
	}
	else//40M
	{
		document.getElementById("n_bandwidth").value="1";
		document.getElementById("n_2040_coexit").value="0";
	}
	
	//Security mode check
	if (securitymode == "WEP") {
		//wep key input value check
		var keylength = document.wireless_setting.wep_key1.value.length;
		
		if (keylength == 0) {
			alert('请输入秘钥');
			document.wireless_setting.wep_key1.focus();
			return false;
		}
		
		if (document.wireless_setting.wep_Select.options.selectedIndex == 0){
			if(keylength != 5 && keylength != 13) {
				alert('请输入5或13个字符');
				return false;
			}
			if(checkInjection(document.wireless_setting.wep_key1.value)== false){
				alert('密码包含非法字符，请重新输入');
				return false;
			}
		} else if (document.wireless_setting.wep_Select.options.selectedIndex == 1){
			if(keylength != 10 && keylength != 26) {
				alert('请输入10或26个字符');
				return false;
			}
			if(checkHex(document.wireless_setting.wep_key1.value) == false){
				alert('密码包含非法字符，请重新输入');
				return false;
			}
		}
		
	}  else if (securitymode == "WPAPSK" || securitymode == "WPA2PSK" ||securitymode == "WPAPSKWPA2PSK" )  {
		var keyvalue = document.wireless_setting.passphrase.value;    
		if (keyvalue.length == 0){
			alert('请输入无线密码');
			return false;
		}

		if (keyvalue.length < 8 || keyvalue.length > 64 ){
			alert('密码需输入8到63个字母或者数字，或者64个十六进制字符');
			return false;
		}

		if (keyvalue.length >= 64 && checkHex(keyvalue) == false) {
			alert('密码需输入8到63个字母或者数字，或者64个十六进制字符');
			return false;
		}
		
		if(checkInjection(document.wireless_setting.passphrase.value) == false){
			alert('密码包含非法字符，请重新输入');
			return false;
		}
	}
	
	//HSSID
	if(document.getElementById("hssid").checked == true) {
	    document.wireless_setting.hssid.value = "0"; 
	} else {
		document.wireless_setting.hssid.value = "";		 
	}

	return true;
}

function afterSubmit(){
   parent.TINY.box.hide();
  parent.location.reload();
}

function security_apply(){
	
	if (check_value() == true) {
		//document.wireless_setting.mssid_0.value = "CMCC_" + document.wireless_setting.mssid.value;
		document.wireless_setting.mssid_0.value = document.wireless_setting.mssid.value;
//		console.log($("#wireless_setting").serialize());
		document.wireless_setting.submit();
		parent.showConfProcess();
	}
}

function ps(){
	if (document.getElementById("passphrase").type=="password"){
		var passwd = WPAPSK1;
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

function show_sub(v) {     
	if(v == 'OPEN') {		
		isHiddenField(1,"hepwd");
		isHiddenField(1,"wepKey");
		isHiddenField(1,"algorithms");
	} else if (v == 'WEP') {			  
		isHiddenField(1,"hepwd");
		isHiddenField(1,"algorithms");
		isHiddenField(0,"wepKey");		
	} else {
		isHiddenField(0,"hepwd");
		isHiddenField(0,"algorithms");
		isHiddenField(1,"wepKey");
		document.wireless_setting.cipher[1].checked = true;
		if (v == 'WPAPSK') {
			document.wireless_setting.cipher[2].disabled = true;
		} else {
			document.wireless_setting.cipher[2].disabled = false;
		}
	}
}  
/*
function change_key_id(input){
	if(input == "秘钥1")
		document.wireless_setting.wep_key_idx=1;
	else if(input == "秘钥2")
		document.wireless_setting.wep_key_idx=2;
	else if(input == "秘钥3")
		document.wireless_setting.wep_key_idx=3;
	else if(input == "秘钥4")
		document.wireless_setting.wep_key_idx=4;
}
*/
//防止缓存
//window.setInterval("refreshCookie()", 1000);

</script>
</head>

<body>
<form method="post" name="wireless_setting" id="wireless_setting" action="/boafrm/wirelessSet">
<input type=hidden name=page value=wireless_setting>
<input type=hidden name=wlan_conf value="2860">
<input type=hidden name=n_bandwidth id="n_bandwidth" value="1"> 
<input type=hidden name=n_2040_coexit id="n_2040_coexit" value="1">
<input type="hidden" id="mssid_0" name="mssid_0" value="" />
<input type="hidden" id="wlan_idx" name="wlan_idx" value="1">
<INPUT type=hidden name="wps_clear_configure_1" value="0">

<div class="int int_1">
	<h1>无线网络设置<span></span></h1>
	<div class="cfg">
			
	<div class="cfgleft">
        <ul>
			<li class="on"><a href="wifisetting1.asp">2.4G</a></li>
			<li><a href="wifisetting2.asp">5G</a></li>
        </ul>
    </div>
	<div class="cfgright">
		<table width="100%" border="0" cellspacing="0" cellpadding="0">
		  <tr>
			<td width="40%"><span>无线网络开关</span></td>
			<td width="42%">
				<div class="selectswitch" id="state">
					<em class="closestate"></em><span><strong id="switch"></strong></span><em class="openstate"></em>
				</div>
				<input type="hidden" id="wifihiddenButton" name="wifihiddenButton" value="1">
			</td>
			<td width="5%">&nbsp;</td>
			<td width="70">&nbsp;</td>
		  </tr>
		  <tr id="networkName">
			<td><span>网络名称</span></td>
			<td>
			  <!--<input class="inp2" disabled="disabled" type="text" maxlength="5" id="mssid_prefix" name="mssid_prefix" value="" style=" width:42px;"/>-->
			  <input class="inp1" type="text" maxlength="27" id="mssid" name="mssid" value="" onkeyup="mykeyup(this)"/>
			</td>
			<td align="center"><input class="checkinp" type="checkbox" id="hssid" name="hssid"/></td>
			<td><i>隐藏网络</i></td>
		  </tr>
		  <tr id ="signalChannel">
			<td width="27%"><span>信道频宽</span></td>
			<td width="42%">
				<select class="selectinp" id="channel" name="channel">
					<option value=0>自动选择</option>
					<option value=1>1</option>
					<option value=2>2</option>
					<option value=3>3</option>
					<option value=4>4</option>
					<option value=5>5</option>
					<option value=6>6</option>
					<option value=7>7</option>
					<option value=8>8</option>
					<option value=9>9</option>
					<option value=10>10</option>
					<option value=11>11</option>
					<option value=12>12</option>
					<option value=13>13</option>					
				</select>
			 </td>			
			<td colspan="2">
				<select class="selectinp_custom1" id="bandwith" name="bandwith">
						<option value=0>20MHz</option>
						<option value=1>20/40MHz</option>
						<option value=2>40MHz</option>
				</select>
			</td>
		  </tr>
		  <tr id="authType">
			<td><span>安全类型</span></td>
			<td>
			  <select class="selectinp" id="security_mode" name="security_mode" onchange="show_sub(this.options[this.options.selectedIndex].value)">
				<option value='OPEN'>Open</option>
				<option value='WEP'>WEP</option>
				<option value='WPAPSK'>WPA</option>
				<option value='WPA2PSK'>WPA2</option>
				<option value='WPAPSKWPA2PSK'>WPA-WPA2 Mixed</option>
			  </select>
			</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		  </tr>
		 <tr id="wepKey">
			<td><span>加密秘钥</span></td>
			<td>
			   <select class="selectinp_custom2" id="wep_default_key" name="wep_default_key" >
					<option value='1'>秘钥1</option>
					<option value='2'>秘钥2</option>
					<option value='3'>秘钥3</option>
					<option value='4'>秘钥4</option>
				</select>
				<input type="text" class="inp3" name="wep_key1" id="wep_key1" value="" />	
			</td>
			<td colspan='2'>
				<select class="selectinp_custom1" id="wep_Select" name="wep_Select">
					<option value="0">ASCII</option>
					<option value="1">16进制</option>
				</select>
			</td>
		  </tr>
		  <tr id="algorithms">
			<td><span>加密算法</span></td>
			<td colspan="3">
				<input class="radiobox" type="radio" name="cipher" value="TKIP">TKIP&nbsp;
				<input class="radiobox" type="radio" name="cipher" value="AES">AES&nbsp;
				<input class="radiobox" type="radio" name="cipher" value="TKIPAES">TKIPAES&nbsp;
			</td>
		  </tr>
		  <tr id="hepwd">
			<td><span>网络密码</span></td>
			<td id="passortext"><input class="inp1" type="text" name="passphrase" id="passphrase"/></td>
			<td align="center"><input class="checkinp" type="checkbox" id="show_password" name="show_password" value="" /></td>
			<td><i>显示密码</i></td>
		  </tr>
		  <tr id="txpower">
			<td><span>信号强度</span></td>
			<td>
			  <select class="selectinp" id="wifi_strength" name="wifi_strength">
				<option value='0'>超强（适用于三居室）</option>
				<option value='1'>标准（适用于二居室）</option>
				<option value='2'>节能（适用于一居室）</option>
			  </select>
			</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			</tr>
	
		</table>
		
		
	</div>
	
</div>
<div class="subpart">
			<ul>
				<li><b onclick="javascript:security_apply();">保存</b></li>
				<li><b class="cancel" onclick="javascript:cancel();">取消</b></li>
			</ul>
		</div>

</div>
</form>
</body>
</html>


