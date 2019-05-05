<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<title>WPS二</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript">

var wpsdisable = '<%getHeInfo("wscDisable", "wlan1")%>';
var wifi_off = '<%getHeInfo("wifi_off", "wlan1")%>';
var wlan_mode = '<%getHeInfo("wlanMode", "wlan1")%>';
var encrypt = '<%getHeInfo("encrypt", "wlan1")%>';
var wpsenable;
var click_num;
var wlan_num = '<%getHeInfo("wlan_num")%>';

function isHiddenField(ishidden,hid_id){
    if(ishidden == 1){
	      document.getElementById(hid_id).style.display = "none";
    } else {
	      document.getElementById(hid_id).style.display = "";
    }
} 


function isCloseWps(close){
	
    if(close == 1) {
	       isHiddenField(1,"wpsconfig");
	  }else {
	       isHiddenField(0,"wpsconfig");
	}
}

function afterSubmit() {
  parent.TINY.box.hide();
  parent.location.reload();
}


$(document).ready(function() {
	click_num = 0;
	if(wpsdisable==1)
		wpsenable = 0;
	else
		wpsenable1 = 1;
	
	if(wpsenable == 0){
	    $("#state").addClass("switchoff");
	    document.wps_setting.WPSEnable.value= 0; 
		isCloseWps(1);
	    
	} else {
		$("#state").removeClass("switchoff");
		document.wps_setting.WPSEnable.value= 1; 
     	isCloseWps(0);
	}
	
	$("#switch").click(function(){
		click_num++;
		if(click_num%2 == 1){
			//turn to open
		  if(wpsenable==0){
			  $("#state").removeClass("switchoff");
			    document.wps_setting.WPSEnable.value= 1;			
			  }else{
			  $("#state").addClass("switchoff");
			    document.wps_setting.WPSEnable.value= 0;		  
			 }
		} else {
		  if(wpsenable == 0){
			  $("#state").addClass("switchoff");
			    document.wps_setting.WPSEnable.value= 0;
			  } else {
			  $("#state").removeClass("switchoff");
			    document.wps_setting.WPSEnable.value= 1;
			 }			
		}
	})

});

function wpsEnableSumbit()
{
	Alert("正在保存设置，请稍候", 10);	
	setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面',25)", 10100);
	$.ajax({
		type: "post",
		url: "/boafrm/wps_setting",     
		data: $("#wps_setting").serialize(),    
		success: function(data){
			//alert("保存成功！");
			window.location.reload();
		},
		error: function(data) {
			//alert("保存失败！\n" + data);
		}
	});
// var wps_enable = 1- parseInt(document.wps_setting.WPSEnable.value);
// isCloseWps(wps_enable);
}

function wpsConfigSubmit()
{
	if(wifi_off==1){
		alert("Wifi处于关闭状态，不支持WPS连接");
		return;
	}
	if(encrypt==1 || encrypt==2){
		alert("该安全模式不支持WPS连接，建议使用WPA2安全模式");
		return;
	}
	
	Alert("正在保存设置，请稍候", 10);	
	setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面',25)", 10100);
	$.ajax({
		type: "post",
		url: "/boafrm/wps_config",
		data: $("#wps_config").serialize(),    
		success: function(data){
			//alert("保存成功！");
		},
		error: function(data) {
			//alert("保存失败！\n" + data);
		}
	});
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
                <li><a class="ifdisabled" href="wireless_guest.asp">2.4G访客网络</a></li>
				<li><a href="wireless_wps.asp">2.4GWPS设置</a></li>
				<li><a href="wireless_mac.asp">2.4G无线访问控制</a></li>
				
				<script language="JavaScript" type="text/javascript">
				if(wlan_num>1){
					document.write("<li><a class=\"ifdisabled\" href=\"wireless_guest_5G.asp\">5G访客网络</a></li>");
					document.write("<li class=\"on\"><a href=\"wireless_wps_5G.asp\">5GWPS设置</a></li>");
					document.write("<li><a href=\"wireless_mac_5G.asp\">5G无线访问控制</a></li>");					
				}							
				</script>	
            </ul>
        </div>

		<div class="cfgright" style="padding-top:12px;">
		<form method="post" name="wps_setting" id="wps_setting" action="/boafrm/wps_setting">
		<input type=hidden name=page value="WPSConfig">
		<input type=hidden name=wlan_conf value="rtdev">
		<input type=hidden id="phyband" name="phyband" value="5">
		
   <div style="border-bottom:1px #efefef solid; padding-bottom:50px;">
	  <table width="100%" border="0" cellspacing="0" cellpadding="0">
		 <tr>
					<td width="25%"><span>WPS开关</span></td>
		    <td width="75%">
			    <div class="selectswitch" id="state">
				   <em class="closestate"></em><span><strong id="switch"></strong></span><em class="openstate"></em>
				</div>
				<input type="hidden" id="WPSEnable" name="WPSEnable" value="1">
			</td>
		 </tr>
	  </table>

   <div class="subpart">
	  <ul>
		 <li><b onclick="wpsEnableSumbit();">保存</b></li>
		 <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
	  </ul>
   </div>
</div>
</form>
<form method="post" name="wps_config" id="wps_config" action="/boafrm/wps_config">
<input type=hidden name=page value="WPS">
<input type=hidden name=wlan_conf value="rtdev">
<input type=hidden id="phyband" name="phyband" value="5">
<input type=hidden name="PINPBCRadio" id="PINPBCRadio" value="2">
   <table width="100%" border="0" cellspacing="0" cellpadding="0" id="wpsconfig">
   	<tr>
				<td style="width:25%;"><span>PIN码</span></td>
				<td width="75%">
				   <% getInfo("wscLoocalPin");%>
				</td>
			</tr>
	  <tr>
				<td style="width:25%;"><span>WPS按键</span></td>
				<td width="75%">
			<span>
				<b onclick="wpsConfigSubmit();" style="display:block; cursor:pointer; width:95px;height: 26px;line-height: 26px; font-weight:normal; padding:0 22px;background:#77d1df; font-size:16px; color:#fff; text-align:center;">开始配置</b>
			</span>
		 </td>
	  </tr>
			</table>
		   
		</form>
		</div>
  </div>
</div>
</body>
</html>

