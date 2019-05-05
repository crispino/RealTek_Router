<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<meta http-equiv="X-UA-Compatible" content="IE=8">
<meta http-equiv="Expires" content="0">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Cache-control" content="no-cache">
<meta http-equiv="Cache" content="no-cache">
<title>WAN设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript">

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

function check_value()
{   
	if (document.wan_adv_setting.vlanEn.value == 1)
	{
		if ( isAllNum(document.wan_adv_setting.vlanid.value) == 0) 
		{
			alert("Vlan ID必须是10-4095！");
			return false;
		}
		if( (document.wan_adv_setting.vlanid.value < 10) || (document.wan_adv_setting.vlanid.value > 4095) )
		{
			alert("Vlan ID必须是10-4095！");
			document.wan_adv_setting.vlanid.focus();
			return false;
		}

		if ( isAllNum(document.wan_adv_setting.vlanpriority.value) == 0) 
		{
			alert("Vlan 优先级必须是0-7！");
			return false;
		}
		
		if(!checkRange(document.wan_adv_setting.vlanpriority.value, 1, 0, 7)) 
		{
			alert("Vlan 优先级必须是0-7！");
			document.wan_adv_setting.vlanpriority.focus();
			return false;
		}
	}

	return true;
		
}

function wan_submit()
{
	if(check_value()==true)
	{
		document.wan_adv_setting.submit(); 
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
<form method="post" name="wan_adv_setting" id="wan_adv_setting" action="/boafrm/formwanconfig">
<input type=hidden name=page value=HeMu_Wan_Adv>
<div class="int">
	<h1>高级设置</h1>
    <div class="cfg">
			<div class="devicelist" style="display: none">
				<h2><b>当前DHCP设备列表</b><span></span></h2>

		</div>
		<div class="cfgleft">
            <ul>
                <li class="on"><a class="ifdisabled" href="WANConfig.asp">WAN设置</a></li>
				<li><a href="DHCPbanding.asp">局域网设置</a></li>
				<li><a class="ifdisabled" href="MACClone.asp">MAC地址克隆</a></li>
				<li><a class="ifdisabled" href="systemtime.asp">系统时间管理</a></li>
				<li><a href="sysupdate.asp">系统升级</a></li>
				<li><a href="restoredefault.asp">恢复出厂设置</a></li>
            </ul>
        </div>
        <div class="cfgright" style="padding-top:10px;padding-left: 12px;">
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="30%" align="left">VLAN</td>
                <td width="70%">
					<div class="selectswitch" id="state1">
						<em class="closestate"></em><span><strong id="switch1"></strong></span><em class="openstate"></em>
						<input type="hidden" id="vlanEn" name="vlanEn" value="0">
					</div>
				</td>
              </tr>
              <tr>
              <td width="30%" align="left">IGMP Snooping</td>
                <td width="70%">
					<div class="selectswitch" id="state2">
						<em class="closestate"></em><span><strong id="switch2"></strong></span><em class="openstate"></em>
						<input type="hidden" id="igmpSnoopingEn" name="igmpSnoopingEn" value="">
					</div>
				</td>
              </tr>
              <tr>
                <td width="30%" align="left">IGMP Proxy</td>
                <td width="70%">
					<div class="selectswitch" id="state3">
						<em class="closestate"></em><span><strong id="switch3"></strong></span><em class="openstate"></em>
						<input type="hidden" id="igmpproxyEn" name="igmpproxyEn" value="">
					</div>
				</td>
              </tr>
              <tr>
                <td width="30%" align="left">UPnP</td>
                <td width="70%">
					<div class="selectswitch" id="state4">
						<em class="closestate"></em><span><strong id="switch4"></strong></span><em class="openstate"></em>
						<input type="hidden" id="UpnpEn" name="UpnpEn" value="">
					</div>
				</td>
              </tr>
            </table>
			<div id="vlanchoose">
				<table width="100%" border="0" cellspacing="0" cellpadding="0">
	              <tr>
	                <td width="30%" align="left">VLAN ID(10-4095)</td>
	                <td width="70%">
						<input class="inp3" name="vlanid" id="vlanid" size="4" type="text">
					</td>
	              </tr>
	              <tr>
	                <td width="30%" align="left">VLAN 优先级(0-7)</td>
	                <td width="70%">
						<input class="inp3" name="vlanpriority" id="vlanpriority" size="1" type="text">
					</td>
	              </tr>
	            </table>
			</div>
			<div class="subpart">
                <ul>
                    <li><b id="subbtn">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
          <input type="hidden" value="/herouter/WANConfig.asp" name="submit-url">
          
        </div>
    </div>
</div>
</form>
<script type="text/javascript">

	function afterSubmit(){
		parent.TINY.box.hide();
		parent.location.reload();  
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
	  var switchcode_vlan = "<% getHeInfo("VlanEnable"); %>";
	  var VLANID_ = "<% getHeInfo("VlanId"); %>";
	  var VLANPriority_ = "<% getHeInfo("VlanPriority"); %>";
	  var switchcode_snoop = "<% getHeInfo("SnoopDisable"); %>";
	  var switchcode_proxy = "<% getHeInfo("ProxyDisable"); %>";
	  var switchcode_upnp = "<% getHeInfo("UPNPEnable"); %>";
	  
	  document.wan_adv_setting.vlanEn.value = switchcode_vlan;
	  document.wan_adv_setting.igmpSnoopingEn.value = switchcode_snoop;
	  document.wan_adv_setting.igmpproxyEn.value = switchcode_proxy;
	  document.wan_adv_setting.UpnpEn.value = switchcode_upnp;
	  
		if(switchcode_vlan==0)
		{
			$("#state1").addClass("switchoff");
			$("#vlanchoose").css("display","none");
			document.wan_adv_setting.vlanid.value =VLANID_;
			document.wan_adv_setting.vlanpriority.value =VLANPriority_;
		}
		else
		{
			$("#state1").removeClass("switchoff");
			$("#vlanchoose").css("display","");
			document.wan_adv_setting.vlanid.value =VLANID_;
			document.wan_adv_setting.vlanpriority.value =VLANPriority_;
		}
		
		if(switchcode_snoop==0)
		{
			$("#state2").addClass("switchoff");
		}
		else
		{
			$("#state2").removeClass("switchoff");
		}
		
		if(switchcode_proxy==0)
		{
			$("#state3").addClass("switchoff");
		}
		else
		{
			$("#state3").removeClass("switchoff");
		}
		
		if(switchcode_upnp==0)
		{
			$("#state4").addClass("switchoff");
		}
		else
		{
			$("#state4").removeClass("switchoff");
		}
		
		$("#switch1").click(function(){
			if(switchcode_vlan==1){
				 $("#state1").addClass("switchoff");
				 switchcode_vlan=0;
				 document.wan_adv_setting.vlanEn.value =switchcode_vlan;
				 
				 $("#vlanchoose").css("display","none");
				}else{
				$("#state1").removeClass("switchoff");
				$("#vlanchoose").css("display","");
				switchcode_vlan=1;
				document.wan_adv_setting.vlanEn.value =switchcode_vlan;
				}
		});
		
		$("#switch2").click(function(){
			if(switchcode_snoop==1){
				 $("#state2").addClass("switchoff");
				 switchcode_snoop=0;
				 document.wan_adv_setting.igmpSnoopingEn.value =switchcode_snoop;
				}else{
				$("#state2").removeClass("switchoff");
				switchcode_snoop=1;
				document.wan_adv_setting.igmpSnoopingEn.value =switchcode_snoop;
				}
		});
		
		$("#switch3").click(function(){
			if(switchcode_proxy==1){
				 $("#state3").addClass("switchoff");
				 switchcode_proxy=0;
				 document.wan_adv_setting.igmpproxyEn.value =switchcode_proxy;
				}else{
				$("#state3").removeClass("switchoff");
				switchcode_proxy=1;
				document.wan_adv_setting.igmpproxyEn.value =switchcode_proxy;
				}
		});

		$("#switch4").click(function(){
			if(switchcode_upnp==1){
				 $("#state4").addClass("switchoff");
				 switchcode_upnp=0;
				 document.wan_adv_setting.UpnpEn.value =switchcode_upnp;
				}else{
				$("#state4").removeClass("switchoff");
				switchcode_upnp=1;
				document.wan_adv_setting.UpnpEn.value =switchcode_upnp;
				}
		});
		
		$("#subbtn").click(function(){
			if(check_value()==false)
				return false;
				
		Alert("正在保存设置，请稍候", 10);	
		setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面', 20)", 10100);	
      $.ajax({
      	type: "post",
        url: "/boafrm/formwanconfig",         
        data: $("#wan_adv_setting").serialize(),    
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
