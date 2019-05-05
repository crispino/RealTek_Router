<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<title>访客网络二</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript">

var wifi_off;
var HideSSID;
var ssid2;
var click_num;
var wlan_guest_idx = '<%getHeInfo("wlan_guest_idx", "wlan1")%>';
var wlan_num = '<%getHeInfo("wlan_num")%>';
if(wlan_guest_idx==1){
	var wifi_off= '<%getHeInfo("wifi_off", "wlan1-va0")%>';
	var HideSSID= '<%getHeInfo("hiddenSSID", "wlan1-va0")%>';
	var ssid2 = '<%getHeInfo("SSID", "wlan1-va0")%>';	
}

function $$$$$(_sId){
 return document.getElementById(_sId);
 }
 
function hide(_sId){
	$$$$$(_sId).style.display = $$$$$(_sId).style.display == "none" ? "" : "none";
 }
 
function afterSubmit() {
  parent.TINY.box.hide();
  parent.location.reload();
}

function isHiddenField(ishidden,hid_id){

    if(ishidden == 1){
	      document.getElementById(hid_id).style.display = "none";

    }else{
	      document.getElementById(hid_id).style.display = "";
    }
} 
function isCloseWIFI(close){
	
    if(close == 1){
		   isHiddenField(1,"networkName");
	  }else if(close == 0){	
		   isHiddenField(0,"networkName");
	  }
	   
} 

$(document).ready(function(){
	click_num=0;
	if(wifi_off == 0){
		$("#state").removeClass("switchoff");
		document.wireless_guest.wifihiddenButton.value= 1; 
		isCloseWIFI(0);    
	}else{
		$("#state").addClass("switchoff");
	    document.wireless_guest.wifihiddenButton.value= 0; 
		isCloseWIFI(1);
	}
	$("#switch").click(function(){
		click_num++;
		if(click_num%2 == 1){
			//turn to open
		  if(wifi_off==1){
			  $("#state").removeClass("switchoff");
			    document.wireless_guest.wifihiddenButton.value = 1;					
			    isCloseWIFI(0);
			  }else{
			  $("#state").addClass("switchoff");
			    document.wireless_guest.wifihiddenButton.value = 0;
			    isCloseWIFI(1);
			    
			 }
		}else{
		  if(wifi_off==1){
			  $("#state").addClass("switchoff");
			    document.wireless_guest.wifihiddenButton.value = 0;

			    isCloseWIFI(1);
			  }else{
			  $("#state").removeClass("switchoff");
			    document.wireless_guest.wifihiddenButton.value = 1;

			    isCloseWIFI(0);
			 }
			
		}
	})
	
	/*var str = new Array();
	 str = ssid2.split("CMCC_");
	 document.wireless_guest.mssid_prefix.value = "CMCC_";
	 document.wireless_guest.mssid.value = str[1];
	*/
	document.wireless_guest.mssid.value = ssid2;
		
	var HideSSIDArray = HideSSID.split(";");
	if(HideSSIDArray[0] == "0"){
      document.getElementById("hssid").checked = false;
	}else{
      document.getElementById("hssid").checked = true;
	}
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
   
	var ssidName;
	ssidName = document.wireless_guest.mssid.value;

/*
	if(document.wireless_guest.wifihiddenButton.value == 0){
	    return true;
	}
*/

	if(document.wireless_guest.mssid.value == "")
	{
		alert("请输入完整的网络名称");
		return false;
	}
	if(ssidName.length < 4 || ssidName.length > 26 )
	{
      alert('输入的网络名称长度需为4-26个英文字母或数字');
			return false;	
	}
	if(document.getElementById("hssid").checked == true)
	    document.wireless_guest.hssid_1.value = "1"; 
	else
		document.wireless_guest.hssid_1.value = "0";  

	//document.wireless_guest.mssid_1.value = "CMCC_" + document.wireless_guest.mssid.value;
	document.wireless_guest.mssid_1.value = document.wireless_guest.mssid.value;
	return true;
}

function wifiGuestSubmit()
{
	if(check_value() == false )
	{
		return;
	}
	else
	{
		//document.wireless_channel.submit();	
		//alert("设置成功");
		Alert("正在保存设置，请稍候", 10);	
		setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面',25)", 10100);
		/*
		 $.ajax({
      	type: "post",
        url: "/boafrm/wireless_guest",     
        data: $("#wireless_guest").serialize(),    
        success: function(data){
        	//alert("保存成功！");
        },
        error: function(data) {
        	//alert("保存失败！\n" + data);
        }
      })
      */
      document.wireless_guest.submit();
	}
}

//防止缓存
//window.setInterval("refreshCookie()", 1000);  
 
</script>
</head>

<body>
<h1 id="lTitle"></h1>
<p id="lIntroduction"></p>
<div class="int">
	<h1>无线设置</h1>
    <div class="cfg">
	
        <div class="cfgleft">
            <ul>
                <li><a href="wireless_guest.asp">2.4G访客网络</a></li>
				<li><a href="wireless_wps.asp">2.4GWPS设置</a></li>
				<li><a href="wireless_mac.asp">2.4G无线访问控制</a></li>
				<script language="JavaScript" type="text/javascript">
				if(wlan_num>1){
					document.write("<li class=\"on\"><a href=\"wireless_guest_5G.asp\">5G访客网络</a></li>");
					document.write("<li><a href=\"wireless_wps_5G.asp\">5GWPS设置</a></li>");
					document.write("<li><a href=\"wireless_mac_5G.asp\">5G无线访问控制</a></li>");					
				}							
				</script>				
            </ul>
        </div>


<form method="post" name="wireless_guest" id="wireless_guest" action="/boafrm/wireless_guest">
<input type=hidden name=page value=HeMu_guest>
<input type=hidden name=wlan_conf value="rtdev">
<input type=hidden name=ssidIndex value="1">
<input type=hidden name=mssid_1 id="mssid_0"  value="">
<input type=hidden name=hssid_1 id="hssid_0"  value="">
<input type="hidden" value="/herouter/wireless_guest_5G.asp" name="submit-url">
  <div class="cfgright" style="padding-top:12px;">	 
    <table width="100%" border="0" cellspacing="0" cellpadding="0">
		  <tr>
			<td width="25%" colspan="2"><span>网络开关</span></td>
			<td width="47%">
				<div class="selectswitch" id="state">
					<em class="closestate"></em><span><strong id="switch"></strong></span><em class="openstate"></em>
				</div>
				<input type="hidden" id="wifihiddenButton" name="wifihiddenButton" value="1">
			</td>
			<td width="8%" colspan="2">&nbsp;</td>
			<td width="20%" colspan="2">&nbsp;</td>
		  </tr>
		  <tr id="networkName">
			<td colspan="2"><span>网络名称</span></td>
			<td>
			  <!--<input class="inp2" disabled="disabled" type="text" maxlength="5" id="mssid_prefix" name="mssid_prefix" value="" style="height:26px;border:1px #d2d2d2 solid;width:45px;font-family:'微软雅黑';color:#666;padding:0 10px;"/>-->
			  <input class="inp3" style="width: 200px;" type="text" maxlength="27" id="mssid" name="mssid" value="" onkeyup="mykeyup(this)"/>
			</td>  
			<td align="center" colspan="2"><input class="checkinp" type="checkbox" id="hssid" name="hssid"/></td>
			<td colspan="2"><i>隐藏网络</i></td>
		</tr>
	 </table>
	<div class="subpart">
        <ul>
           <li><b onclick="wifiGuestSubmit();">保存</b></li>
           <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
        </ul>
    </div>
 </div>
</form>
  </div>
</div>
</body>
</html>

