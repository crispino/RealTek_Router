<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<title>防火墙管理IPfilter</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript">
var AccessControlList = new Array();
var AccessPolicy = 0;
var http_request = false;

var all_str = "<!--#exec cmd=\"web 2860 firewall HeMu_IPfilterlist\"-->";

var DelControlList = new Array();

var DelAllList = new Array();

function checkIpAddr(field, ismask)
{
	var ip_addr = field.split(".");
	var i;

	if (field == "")
	{
		//alert("Error. IP address is empty.");
		
		return false;
	}
	if (ip_addr.length != 4)
	{
			//alert('IP adress format error, please keyin like 10.10.10.254.');
			
			return false;
	}
	for (i=0; i<4 ; i++)
	{
		if (isNaN(ip_addr[i]) == true)
		{
			//alert('It should be a [0-9] number.');
			
			return false;
		}
	}
	if (ismask) {
		for (i=0; i<4; i++) 
		{
			if ((ip_addr[i] > 255) || (ip_addr[i] < 0))
			{
				//alert('Mask IP adress format error, please keyin like 255.255.255.0.');
				
				return false;
			}
		}
	}
	else {
		for (i=0; i<3; i++) 
		{
			if ((ip_addr[i] > 255) || (ip_addr[i] < 0))
			{
				//alert('IP adress format error, please keyin like 10.10.10.254.');
				
				return false;
			}
		}
		if ((ip_addr[i] > 254) || (ip_addr[i] < 0))
		{
				//alert('IP adress format error.');
				
				return false;
		}
	}

	return true;
}

function makeRequest(url, content, handler) {
	http_request = false;
	if (window.XMLHttpRequest) { // Mozilla, Safari,...
		http_request = new XMLHttpRequest();
		if (http_request.overrideMimeType) {
			http_request.overrideMimeType('text/xml');
		}
	} else if (window.ActiveXObject) { // IE
		try {
			http_request = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e) {
			try {
			http_request = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e) {}
		}
	}
	if (!http_request) {
		alert('Giving up :( Cannot create an XMLHTTP instance');
		return false;
	}
	http_request.onreadystatechange = handler;
	if (document.all)//IE
		http_request.open('POST', url, true);
	else
		http_request.open('POST', url, false);
	http_request.send(content);
}

function addnewIP()
{
	document.getElementById("IPadd").style.display = "";
}
function parseAllData(str)
{
    var enable;
	var filter_type;
	//var enable = "<!--#exec cmd=\"web 2860 nvram IPFilterEn\"-->";
	if ( <% getHeInfo("ipfilterEnable"); %> ){
	    if ( <% getHeInfo("currentIpfiltermode"); %> ){
			enable = 1;
		}
		else
		    enable = 2;
	}
	else 
		enable = 0;

	//var filter_type = "<!--#exec cmd=\"web 2860 nvram filter_type\"-->";
	if ( <% getHeInfo("currentIpfiltertype"); %> ){
	    filter_type = 1;
	}
	else 
		filter_type = 0;
	
	var filter_protocol = "<% getHeInfo("currentIpfilterprotocol"); %>";

	document.MACBasicSettings.apselect_0.value = enable;
	document.MACBasicSettings.filter_type.value = filter_type;
	document.MACBasicSettings.filter_protocol.value = filter_protocol;
	//if(currentMac)
	//	document.getElementById('macinfo').innerHTML='当前使用设备MAC地址：'+currentMac;
	
	var switchcode=0;
	if(document.MACBasicSettings.apselect_0.value == 0){
		$("#state").addClass("switchoff");
		$(".swindow").css("display","none");
		$(".macinfo").css("display","none");
		document.getElementById("filter_mode").style.display= "none";
		document.getElementById("tr_protocol").style.display= "none";
	}	
	else if(document.MACBasicSettings.apselect_0.value == 1){ 
		$("#state").removeClass("switchoff");
		document.getElementById("filter_mode").style.display= "";
		if (document.MACBasicSettings.filter_type.value == 0)
		{
			document.getElementById("tr_protocol").style.display= "none";
			$(".swindow").css("display","");
			//$(".macinfo").css("display","");
			$("#btndown").removeClass("downon");
			$("#btndown").addClass("downoff");
			$("#disptxt").removeClass("opst2");
			$("#btnup").removeClass("upoff");
			$("#btnon").addClass("upon");
			document.getElementById("allow").style.display= "";
			document.getElementById("reject").style.display= "none"
		}
		else if (document.MACBasicSettings.filter_type.value == 1)
		{
			$(".swindow").css("display","none");
			document.getElementById("tr_protocol").style.display= "";
		}
	}	
	else if(document.MACBasicSettings.apselect_0.value == 2){
		$("#state").removeClass("switchoff");
		document.getElementById("filter_mode").style.display= "";
		if (document.MACBasicSettings.filter_type.value == 0)
		{
			$(".swindow").css("display","");
			//$(".macinfo").css("display","");
			$("#btnup").removeClass("upon");
			$("#btnup").addClass("upoff");
			$("#disptxt").addClass("opst2");
			$("#btndown").removeClass("downoff");
			$("#btndown").addClass("downon");
			document.getElementById("allow").style.display= "none";
			document.getElementById("reject").style.display= ""
		}
		else if (document.MACBasicSettings.filter_type.value == 1)
		{
			$(".swindow").css("display","none");
			document.getElementById("tr_protocol").style.display= "";
			document.getElementById("filter_protocol").style.display= "";
		}
	}

	var i;
	var entries = new Array();
	var every = new Array();
	
	var get_str = "<% getHeInfo("getIpfilterRules"); %>";

	if(get_str.length){
		/* any/0,0,0,0,any/0,0,0,0,5,0,,11:22:33:11:22:33;any/0,0,0,0,192.168.0.123,0,0,0,5,0,,;any/0,0,0,0,any/0,0,0,0,5,0,,11:22:33:44:55:11 */
		entries = get_str.split(";");
		
		for(i=0; i < entries.length && entries[i] != ""; i++){
			//alert("i = " +i);
			
			/* every[4] is dip_1, maybe 0, any, any/0 ,every[12] is current mode , every[13] is entry mode*/
			every = entries[i].split(",");
			if(every[4] != 0 && every[4] != "any" && every[4] != "any/0" && every[4] !="0.0.0.0" && every[12] == every[13])
			{
				DelAllList.push(i);
				document.getElementById("newap_"+i).value = every[4];
				document.getElementById("em_"+i).style.display = "";
			}
			else
			{	
				document.getElementById("em_"+i).style.visibility = "hidden";
				document.getElementById("em_"+i).style.display = "none";
			}
		}
	}
	
	
}

function Onload()
{
	parseAllData();
}

function sortNum(a,b) 
{ 
    return (a-b); 
} 

function afterSubmit(){
  parent.TINY.box.hide();
  parent.location.reload();  
}
function submitmac()
{
	var listNum = new Array();
	var Enable = document.getElementById("apselect_0").value;
	var filter_type = document.MACBasicSettings.filter_type.value;
	var filter_protocol = document.MACBasicSettings.filter_protocol.value;

	if(document.getElementById("apselect_0").value == "0" )
	{
	    document.MACBasicSettings.submit();
//		makeRequest("/cgi-bin/firewall.cgi", "firewall=HeMu_IPFilterDelete&" + DelAllList.join(";") + "&apselect_0=" + Enable, null);
		DelAllList.splice(0, DelAllList.length);
	}
	else
	{
		/* 基于IP过滤 */
		if (filter_type == 0)
		{
			if (Enable == 1)
				document.MACBasicSettings.action.value = "Accept";
			else if (Enable == 2)
				document.MACBasicSettings.action.value = "Drop";
			
//			if (DelControlList.length > 0)
//			{
//				listNum = DelControlList.sort(sortNum);
//				makeRequest("/cgi-bin/firewall.cgi", "firewall=HeMu_IPFilterDelete&" + listNum.join(";") + "&apselect_0=" + Enable, null);
//				DelControlList.splice(0, DelControlList.length);
//			}
			
			if(document.getElementById("IPadd").style.display == ''){
				var ipaddress = document.getElementById("dip_address").value;
				if( ipaddress == ""){
					alert(" 请输入IP地址!\n");
					return false;
				}else{
					if(ipaddress.indexOf('-') != -1){
						var ipArr = ipaddress.split('-');
						
						if( checkIpAddr(ipArr[0], false) && checkIpAddr(ipArr[1], false) ){
							var ad1 = ipArr[0].split('.');
							var ad2 = ipArr[1].split('.'); 
							if(ad1[0]!=ad2[0] || ad1[1]!=ad2[1] || ad1[2]!=ad2[2]){
								alert("请输入同一网段IP\n");
								document.getElementById("dip_address").focus();
								return false;
							}else if(ad1[3] >= ad2[3]){
								alert("起始IP不能大于终止IP\n");
								document.getElementById("dip_address").focus();
								return false;
							}else{
								document.MACBasicSettings.dip_address_start.value = ipArr[0];
								document.MACBasicSettings.dip_address_end.value = ipArr[1];
							}
							
						}else{
							alert("无效的IP地址!\n");
							document.getElementById("dip_address").focus();
							return false;
						}
					}else{
						if(! checkIpAddr(ipaddress, false) )
						{
							alert("无效的IP地址!\n");
							document.getElementById("dip_address").focus();
							return false;
						}
					}
				}
			}
			document.MACBasicSettings.submit();
		}
		/* 基于协议过滤 */
		else if (filter_type == 1)
		{
		    document.MACBasicSettings.submit();
//			makeRequest("/cgi-bin/firewall.cgi", "firewall=HeMu_PROFilter&" + DelAllList.join(";") + "&apselect_0=" + Enable + "&filter_type=" + filter_type + "&filter_protocol=" + filter_protocol, null);
			DelAllList.splice(0, DelAllList.length);
		}
	}

	
	
	//alert("正在设置请稍等.....");
	Alert("正在保存设置，请稍候", 5);
	setTimeout("Alert('设置成功', 3)", 5000);
	setTimeout('parseAllData();location.reload();', 3000);
}
function deleteAccessPolicyListHandler()
{
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
				window.location.reload(false);
		} else {
			//alert('There was a problem with the request.'+http_request.readyState+':'+http_request.status);
		}
	}	
}

function del(iid)
{
	//makeRequest("/goform/APDeleteAccessPolicyList", "0,"+iid, deleteAccessPolicyListHandler);	
	document.getElementById("em_"+iid).style.visibility = "hidden";
	document.getElementById("em_"+iid).style.display = "none";
	document.getElementById("newap_"+iid).value = "disabled";
	DelControlList.push(iid);
}

function isHiddenField(ishidden,hid_id){

    if(ishidden == 1){
	      document.getElementById(hid_id).style.display = "none";
    }else{
	      document.getElementById(hid_id).style.display = "";
    }
} 
  
function show_sub(v) {     
	if(v == '0') {		
		isHiddenField(1,"tr_protocol");
		isHiddenField(0,"tr_swindow");
		if(document.MACBasicSettings.apselect_0.value == 1){
			$("#btndown").removeClass("downon");
			$("#btndown").addClass("downoff");
			$("#disptxt").removeClass("opst2");
			$("#btnup").removeClass("upoff");
			$("#btnon").addClass("upon");
			document.getElementById("allow").style.display= "";
			document.getElementById("reject").style.display= "none"
		}
		else if(document.MACBasicSettings.apselect_0.value == 2){
		    $("#btnup").removeClass("upon");
			$("#btnup").addClass("upoff");
			$("#disptxt").addClass("opst2");
			$("#btndown").removeClass("downoff");
			$("#btndown").addClass("downon");
			document.getElementById("allow").style.display= "none";
			document.getElementById("reject").style.display= ""
		}
	} else 	{
		isHiddenField(0,"tr_protocol");
		isHiddenField(1,"tr_swindow");
	}
}  

//防止缓存
//window.setInterval("refreshCookie()", 1000);   

</script>
</head>

<body onLoad="Onload()">
<div class="int">
	<h1>防火墙管理</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
				<li><a href="ALGconfig.asp">ALG</a></li>
                <li class="on"><a href="IPfilter.asp">IP过滤</a></li>
				<li><a href="URLfilter.asp">网址过滤</a></li>
				<li><a href="MACfilter.asp">MAC过滤</a></li>
            </ul>
        </div>
        <form method="post" name="MACBasicSettings" id="MACBasicSettings" action="/boafrm/heUIIpfilter">
		<input type=hidden name=firewall value=HeMu_IPfilter>
		<input type=hidden name=wlan_conf value="2860">
        <input type="hidden" id="apselect_0" name="apselect_0" >
        <input type="hidden" id="dip_address_start" name="dip_address_start" value=""> 
        <input type="hidden" id="dip_address_end" name="dip_address_end" value=""> 
        <table width="500" border="1" cellpadding="2" cellspacing="1" style="display: none">
        <tr>
            <td class="title" colspan="4" id="portFilterSet">IP/Port Filter Settings</td>
        </tr>
        
        <tr>
            <td class="head" colspan="2" id="portFilterMac">
                Mac address
            </td>
            <td colspan="2">
                 <input type="text" size="18" name="mac_address">
            </td>
        </tr>
        
        <tr>
            <td class="head" colspan="2" id="portFilterDIPAddr">
                Dest IP Address
            </td>
            <td colspan="2">
                <!-- 
                <input type="text" size="16" name="dip_address">
                -->
                <!-- we dont support ip range in kernel 2.4.30 
                -<input type="text" size="16" name="dip_address2">
                -->
            </td>
        </tr>
        
        <tr>
            <td class="head" colspan="2" id="portFilterSIPAddr">
                Source IP Address
            </td>
            <td colspan="2">
                <input type="text" size="16" name="sip_address">
                <!-- we dont support ip range in kernel 2.4.30 
                -<input type="text" size="16" name="sip_address2">
                -->
            </td>
        </tr>
        
        <tr>
            <td class="head" colspan="2" id="portFilterProtocol">
                Protocol
            </td>
            <td colspan="2">
                <select onChange="protocolChange()" name="protocol" id="procotol">
                <option value="None">None</option>
                <option value="TCP">TCP</option>
                <option value="UDP">UDP</option>
                <option value="ICMP">ICMP</option>
                </select>&nbsp;&nbsp;
            </td>
        </tr>
        
        <tr>
            <td class="head" colspan="2" id="portFilterDPortRange">
                Dest. Port Range
            </td>
            <td colspan="2">
                <input type="text" size="5" name="dFromPort" id="dFromPort">-
                <input type="text" size="5" name="dToPort" id="dToPort">
            </td>
        </tr>
        
        
        <tr>
            <td class="head" colspan="2" id="portFilterSPortRange">
                Src Port Range
            </td>
            <td colspan="2">
                <input type="text" size="5" name="sFromPort" id="sFromPort">-
                <input type="text" size="5" name="sToPort" id="sToPort">
            </td>
        </tr>
        
        <tr>
            <td class="head" colspan="2" id="portFilterAction">
                Action
            </td>
            <td colspan="2">
                <select onChange="actionChanged()" name="action_t">
                <option value="Drop" id="portFilterActionDrop">Drop</option>
                <option value="Accept" id="portFilterActionAccept">Accept</option>
                </select>
            </td>
            <td><input type="text" name="action" value="Drop"></td>
        </tr>
        
        <tr>
            <td class="head" colspan="2" id="portFilterComment">
                Comment
            </td>
            <td colspan="2">
                <input type="text" name="comment" size="16" maxlength="32">
            </td>
        </tr>
        </table>

        <div class="cfgright" style="padding-top:10px;padding-left: 12px;">
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="8%" align="left">开关</td>
                <td width="90%">
									<div class="selectswitch" id="state">
										<em class="closestate"></em><span><strong id="switch"></strong></span><em class="openstate"></em>
									</div>
								</td>
              </tr>
            </table>
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr  id="filter_mode" name="filter_mode">
               <td width="20%" align="left">IP过滤类型</td>
			   <td>
				<select id="filter_type" name="filter_type" class="selectinp_custom3" onChange="show_sub(this.options[this.options.selectedIndex].value)">
					<option value=0>基于IP过滤</option>
					<option value=1>基于协议过滤</option>
				</select>
               </td>
              </tr>
              <tr style="display: none" id="tr_protocol">
               <td align="left">协议</td>
			   <td>
				<select class="selectinp_custom3" id="filter_protocol" name="filter_protocol">
					<option value=0>ICMP+ARP</option>
					<option value=1>ICMP</option>
					<option value=2>ARP</option>
				</select>
			   </td>
              </tr>
            </table>
			<div class="swindow" id="tr_swindow" style="display: ">
				<div class="sleft">
					<div class="fixblock">
						<div class="up" id="btnup"></div>
						<div class="op" id="disptxt">
							<p>白名单设置</p>
							<p>黑名单设置</p>
						</div>
						<div class="down" id="btndown"></div>
					</div>
				</div>
				<div class="sright">
					<div class="statedisp">
						<div id="allow" style="display: ">
							<strong>当前设置为允许以下IP地址访问</strong><span onClick="addnewIP();">新增IP地址</span>
						</div>
						<div id="reject" style="display: none">
							<b>当前设置为不允许以下IP地址访问</b><span onClick="addnewIP();">新增IP地址</span>
						</div>
					</div>
					<div class="maclist maclist1">
						<ul>
<script language="JavaScript" type="text/javascript">	
var i ;
for (i = 0; i <= 32; i++)
{

	document.write("<li id=\"em_"+i+"\" style=\"display: none\"><em><input name=\"select_"+i+"\" value=\"enabled\" id=\"newap_"+i+"\" size=16 maxlength=20 bgcolor=\"#e1e1e1\" readonly ></em><span onclick=\"del("+i+");\" ></span></li>")	;
}							
</script>		
						<li id="IPadd" name="IPadd"	style="display:none"><em><input type="text" id="dip_address" name="dip_address" maxlength=31 > </em></li>			
						</ul>
					</div>
				</div>
			</div>
			<div class="macinfo" id="macinfo" style="display: none"></div>
            <div class="subpart">
                <ul>
                    <li ><b onClick="javascript:submitmac();">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
			<input type="hidden" value="/herouter/IPfilter.asp" name="submit-url">

        </div>
      </form>
    </div>
</div>
<script type="text/javascript">
	//document.MACBasicSettings.apselect_0.value = AccessPolicy
$(document).ready(function(){	
	$("#switch").click(function(){
		if(document.MACBasicSettings.apselect_0.value != 0){
			 $("#state").addClass("switchoff");
			 $(".swindow").css("display","none");
			 $(".macinfo").css("display","none");
			 document.getElementById("filter_mode").style.display= "none";
			 document.getElementById("tr_protocol").style.display= "none";
			 document.MACBasicSettings.apselect_0.value = 0;
		}else{
			var enable;
			if ( <% getHeInfo("currentIpfiltermode"); %> ){
				enable = 1;
			}
			else
		    	enable = 2;
			$("#state").removeClass("switchoff");
			document.MACBasicSettings.apselect_0.value = enable;
			document.getElementById("filter_mode").style.display= "";
			if (document.MACBasicSettings.filter_type.value == 0)
			{
                if(document.MACBasicSettings.apselect_0.value == 1){
			    document.getElementById("tr_protocol").style.display= "none";
			    $(".swindow").css("display","");
				//$(".macinfo").css("display","");
				$("#btndown").removeClass("downon");
				$("#btndown").addClass("downoff");
				$("#disptxt").removeClass("opst2");
				$("#btnup").removeClass("upoff");
				$("#btnon").addClass("upon");
				document.getElementById("allow").style.display= "";
				document.getElementById("reject").style.display= "none"
				}else{
				document.getElementById("tr_protocol").style.display= "none";
				$(".swindow").css("display","");
				//$(".macinfo").css("display","");
				$("#btnup").removeClass("upon");
				$("#btnup").addClass("upoff");
				$("#disptxt").addClass("opst2");
				$("#btndown").removeClass("downoff");
				$("#btndown").addClass("downon");
				document.getElementById("allow").style.display= "none";
				document.getElementById("reject").style.display= "";
				}
			}
			else if (document.MACBasicSettings.filter_type.value == 1)
			{
				$(".swindow").css("display","none");
				document.getElementById("tr_protocol").style.display= "";
			}
		}
	});	
	$("#btnup").click(function(){
		if(document.MACBasicSettings.apselect_0.value == 1){
			$("#btnup").removeClass("upon");
			$("#btnup").addClass("upoff");
			$("#disptxt").addClass("opst2");
			$("#btndown").removeClass("downoff");
			$("#btndown").addClass("downon");
			document.MACBasicSettings.apselect_0.value = 2;
			document.getElementById("allow").style.display= "none";
			document.getElementById("reject").style.display= "";

			var i;
			var entries = new Array();
			var every = new Array();
	
			var get_str = "<% getHeInfo("getIpfilterRules"); %>";

			if(get_str.length){
				/* any/0,0,0,0,any/0,0,0,0,5,0,,11:22:33:11:22:33;any/0,0,0,0,192.168.0.123,0,0,0,5,0,,;any/0,0,0,0,any/0,0,0,0,5,0,,11:22:33:44:55:11 */
				entries = get_str.split(";");
		
				for(i=0; i < entries.length && entries[i] != ""; i++){
			
					/* every[4] is dip_1, maybe 0, any, any/0 ,every[12] is current mode , every[13] is entry mode*/
					every = entries[i].split(",");
					if(every[4] != 0 && every[4] != "any" && every[4] != "any/0" && every[4] !="0.0.0.0" && every[13] == "0")
					{
						DelAllList.push(i);
						document.getElementById("newap_"+i).value = every[4];
						document.getElementById("em_"+i).style.visibility = "";
						document.getElementById("em_"+i).style.display = "";
					}
					else
					{	
						document.getElementById("em_"+i).style.visibility = "hidden";
						document.getElementById("em_"+i).style.display = "none";
					}
				}
			}
		}
	});
	$("#btndown").click(function(){
		if(document.MACBasicSettings.apselect_0.value == 2){
			$("#btndown").removeClass("downon");
			$("#btndown").addClass("downoff");
			$("#disptxt").removeClass("opst2");
			$("#btnup").removeClass("upoff");
			$("#btnon").addClass("upon");
			document.MACBasicSettings.apselect_0.value = 1 ;
			document.getElementById("allow").style.display= "";
			document.getElementById("reject").style.display= "none";

			var i;
			var entries = new Array();
			var every = new Array();
	
			var get_str = "<% getHeInfo("getIpfilterRules"); %>";

			if(get_str.length){
				/* any/0,0,0,0,any/0,0,0,0,5,0,,11:22:33:11:22:33;any/0,0,0,0,192.168.0.123,0,0,0,5,0,,;any/0,0,0,0,any/0,0,0,0,5,0,,11:22:33:44:55:11 */
				entries = get_str.split(";");
		
				for(i=0; i < entries.length && entries[i] != ""; i++){
			
					/* every[4] is dip_1, maybe 0, any, any/0 ,every[12] is current mode , every[13] is entry mode*/
					every = entries[i].split(",");
					if(every[4] != 0 && every[4] != "any" && every[4] != "any/0" && every[4] !="0.0.0.0" && every[13] == "1")
					{
						DelAllList.push(i);
						document.getElementById("newap_"+i).value = every[4];
						document.getElementById("em_"+i).style.visibility = "";
						document.getElementById("em_"+i).style.display = "";
					}
					else
					{	
						document.getElementById("em_"+i).style.visibility = "hidden";
						document.getElementById("em_"+i).style.display = "none";
					}
				}
			}
		}
	});
});
</script>
</body>
</html>
