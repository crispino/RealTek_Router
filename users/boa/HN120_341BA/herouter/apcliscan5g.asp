<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<meta http-equiv="X-UA-Compatible" content="IE=8">
<meta http-equiv="Expires" content="0">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Cache-control" content="no-cache">
<meta http-equiv="Cache" content="no-cache">
<title>路由模式设置-中继</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript" src="js/tinybox.js"></script>
<script type="text/javascript">

var curr_sel_id = 0;

function afterSubmit(){
  parent.TINY.box.hide();
  parent.location.reload();
}

function reload_main()
{
  top.location.href="/main.html";
}
function switch_conType(){

	if(document.heUiopmode.opMode[2].checked)
	{
		  	document.getElementById("wirelesslistdiv").style.display="block";
				document.getElementById("submit1").style.display = "none";
				document.getElementById("apclira0div").style.display = "none";
		  	document.getElementById("rescan").style.display="block";
	}
	else
	{
		  	document.getElementById("wirelesslistdiv").style.display="none";
		  	document.getElementById("submit1").style.display="block";
		  	document.getElementById("apclira0div").style.display="none"; 	
  	 	 document.getElementById("rescan").style.display="none"; 
	}
		
}
function wanmodeSetting_apply(){
	
	if (document.heUiopmode.opMode[2].checked == true)
	{
		Alert("正在保存设置，请稍候", 120); 
	  $('#connect').html('连接中...')
	
		
		document.getElementById("apcliEnbl").value="1";
		document.getElementById("natEnbl").value="0"; 
		
		$.ajax({
			type: "post",
			url: "/boafrm/heUiOpMode",
			data: $("#heUiopmode").serialize(),
			success: function(data){
				//alert("保存成功！");
			        setTimeout("reload_main()",3000);
			},
			error: function(data) {
				//alert("保存失败！\n" + data);
				setTimeout("reload_main()",10000);
			}
		})
	}
	else
	{
		$("#opmodeSaveBtn").html("正在保存中...");
		Alert("正在保存设置，请稍候", 30); 
		if( document.heUiopmode.opMode[0].checked == true)
		{
			document.getElementById("apcliEnbl").value="0";
			document.getElementById("natEnbl").value="1";
		}
		$.ajax({
	      	type: "post",
	        url: "/boafrm/heUiOpMode",     
	        data: $("#heUiopmode").serialize(),    
	        success: function(data){
	        	parent.showChangeOpmode();
	        },
	        error: function(xmlrequest,text,error) {
	        	//alert("保存失败！\n" +xmlrequest + "text "+text+" error "+error);
	        }
      })

	}
}

function apcliSSID()
{
	var rowSel = $("input[name='rowSelect']:checked").attr("id");     
			if(!rowSel) {
				alert("请选择网络");
				return;
			}
	var tr_id = rowSel.substring(6);    	
	var channel = document.getElementById("channel"+tr_id).innerText;    
	if(channel > 14)    
	{       
		document.getElementById("wlan_conf").value = "rtdev";    
	}    
	else    
	{    	
		document.getElementById("wlan_conf").value = "2860";    
	}	
	var ssid = document.getElementById("ssid"+tr_id).innerText;	
	var security = document.getElementById("security" + tr_id).innerText;
	var security_select = document.getElementById("apcli_mode");
	var apcli_enc_select = document.getElementsByName("apcli_enc");
	var bssid = document.getElementById("bssid"+tr_id).innerText;
	curr_sel_id = tr_id;

    document.getElementById("rescan").style.display="none";
	document.getElementById("apclira0div").style.display="block"; 	
	document.getElementById("wirelesslistdiv").style.display="block";
	document.getElementById("wirelesslistdiv").style.display="none";
	document.getElementById("apcli_ssid").value=ssid;
//	document.getElementById("algorithms").value=AES;
	document.getElementById("ra0channel").value=channel;
	document.getElementById("bssid").value=bssid;

			if(security.indexOf("NONE") != -1) {
				security_select.value = "OPEN";
				
				show_sub("OPEN");

			} else if(security.indexOf("WEP") != -1) // Default shared wep, ASCII
			{

				security_select.value = "WEP";
				
				show_sub("WEP");
			} else if(security.indexOf("WPAPSK") != -1 && security.indexOf("WPA2") == -1) {

				security_select.value = "WPAPSK";
				show_sub("WPAPSK");

				if(security.indexOf("TKIP") != -1 && security.indexOf("AES") == -1) {
					apcli_enc_select[0].checked = true;
				} else if(security.indexOf("AES") != -1 && security.indexOf("TKIP") == -1) {

					apcli_enc_select[1].checked = true;
				}else{
					apcli_enc_select[2].checked = true;
				}

			} else if(security.indexOf("WPA2") != -1 && security.indexOf("WPA1PSK") == -1) {
				security_select.value = "WPA2PSK";
				show_sub("WPA2PSK");
				
				if(security.indexOf("TKIP") != -1 && security.indexOf("AES") == -1) {
					apcli_enc_select[0].checked = true;
				} else if(security.indexOf("AES") != -1 && security.indexOf("TKIP") == -1) {

					apcli_enc_select[1].checked = true;
				}else{
					apcli_enc_select[2].checked = true;
				}

			}else{
				security_select.value = "WPA1PSKWPA2PSK";
				show_sub("WPA2PSK");
				
				if(security.indexOf("TKIP") != -1 && security.indexOf("AES") == -1) {
					apcli_enc_select[0].checked = true;
				} else if(security.indexOf("AES") != -1 && security.indexOf("TKIP") == -1) {

					apcli_enc_select[1].checked = true;
				}else{
					apcli_enc_select[2].checked = true;
				}
			}
				
				if(channel<20)
					document.getElementById("wlan_idx").value="0";
				else
					document.getElementById("wlan_idx").value="1";	

		}

function reselect()
{
	parent.routerMode();
}

function back_step()
{
    document.getElementById("wirelesslistdiv").style.display="block";
    document.getElementById("rescan").style.display="block";
    document.getElementById("submit1").style.display="none";
    document.getElementById("apclira0div").style.display="none";   
	if(curr_sel_id!=0)
	{
		document.getElementById("select"+curr_sel_id).checked=true;
	}
}

function initValue(){
  
  var opmode=<% getIndex("opMode"); %>;
  var repeaterMode = <% getHeInfo("repeaterMode"); %>; 
  
  if(opmode==0){
      $("input[name='opMode']")[0].checked=true;  
      document.getElementById("wirelesslistdiv").style.display="none";    
  	  document.getElementById("rescan").style.display="none";
  }else if(opmode==1 && repeaterMode!=1){ 
  	  $("input[name='opMode']")[1].checked=true;
  	  document.getElementById("wirelesslistdiv").style.display="none";
  	  document.getElementById("rescan").style.display="none";
  }else if(opmode==1 && repeaterMode==1){ 
  	  $("input[name='opMode']")[2].checked=true;
  	  document.getElementById("wirelesslistdiv").style.display="block";
  	  document.getElementById("submit1").style.display="none";
  	  document.getElementById("rescan").style.display="block";
  }

	document.getElementById("apclira0div").style.display="none"; 
	
	if(curr_sel_id!=0)
	{
		document.getElementById("select"+curr_sel_id).checked=true;
	}
	return true;
}

function cancel(){
	parent.TINY.box.hide();
}

function show_sub(v) {  

			if(v == "OPEN") {
				
				document.getElementById("wirelesskey").style.display = "none";
				document.getElementById("apcli_enc").style.display = "none";
			} else if(v == "WEP") {
				
				document.getElementById("wirelesskey").style.display = "block";
				document.getElementById("apcli_enc").style.display = "none";
			} else if(v == "WPAPSK") {
				
				document.getElementById("wirelesskey").style.display = "block";
				document.getElementById("apcli_enc").style.display = "block";
			} else if(v == "WPA2PSK") {
		
				document.getElementById("wirelesskey").style.display = "block";
				document.getElementById("apcli_enc").style.display = "block";
			}
		}
		
	

	
	</script>
</head>

<body onLoad="initValue()">
<input type=hidden name=page value=apclient>
<input type=hidden name=apcliEnbl id=apcliEnbl value=0>
<input type=hidden name=natEnbl id=natEnbl value=1>

<div class="int int_1">
	<h1>路由模式设置<span>*请选择您的路由模式</span></h1>
    <div class="cfg">
        
		   <form method="post" name="heUiopmode" id="heUiopmode" action="/boafrm/heUiOpMode">
           <table width="100%" border="0" cellspacing="0" cellpadding="0" style="padding-left: 50px;">
              <tr>
                <td width="35%" >
                <input class="radiobox" type="radio" name="opMode" onClick="switch_conType()" value="0">路由模式(Router)
                </td>
                <td width="35%" >
                <input class="radiobox" type="radio" name="opMode"  onClick="switch_conType()"  value="1">桥接模式（Bridge）
								</td>
                <td width="30%" >
                	<input class="radiobox" type="radio" name="opMode" onClick="switch_conType()"  value="3">中继模式(Repeater)		
                </td>
              </tr>
            </table>
	        <div class="wirelesslistright" style="height:330px;overflow-x:auto;overflow-y:auto;" id="wirelesslistdiv">					
				<table id="wirelesslist"  width="610" border="1" cellpadding="2" cellspacing="1" height="400">
				<tr style="background-color: #E8F8FF;color: #333;"> 
					  <td width="50" >信道</td>					   		
					  <td >SSID</td>
					  <td width="210" >安全类型</td>
					  <td width="100" >信号强度(%)</td>
						<td width="50" >选择</td>
						<td style="display:none"></td>
				</tr>
					<% draw_wlSiteSurveyTbl(); %>   					
				</table>
			</div>
            <div class="subpart" id="rescan" style="padding-left: 150px;">
                <table>
                    <tr>
                        <td><b class="reselect" onClick="reselect()">重新扫描</b></td>
                        <td><b class="reselect" id="nextStep" onClick="apcliSSID()">下一步</b></td>
                    </tr>
                </table>
            </div>	
					<div style="display:none;padding-top:20px" id="apclira0div">
						<input type=hidden id="ra0channel" name="ra0channel" value="">
				<!--input type=hidden id="apcli_mode" name="apcli_mode" value=""-->
				<!--input type=hidden id="apcli_wpapsk" name="apcli_wpapsk" value=""-->
				<input type=hidden id="apcli_default_key" name="apcli_default_key" value="1">
				<!--input type=hidden id="apcli_key1" name="apcli_key1" value=""-->
				<input type=hidden id="apcli_key1type" name="apcli_key1type" value="1">
				<input type=hidden id="wlan_conf" name="wlan_conf" value="rtdev">
				<input type=hidden id="bssid" name="bssid" value="">
				<input type=hidden id="wlan_idx" name="wlan_idx" value="">
								
			    <table width="80%" border="0" cellspacing="0" cellpadding="0">
							<tr id="wifissid" style="display:block;">
								<td style="width:218px;text-align: right;padding-right: 20px;"><span>网络名称</span></td>
				        <td style="width:218px"><input class="inp1" name="apcli_ssid" id="apcli_ssid" value="" readonly="readonly"</td>
							</tr>
							<tr id="authType" style="display:block;">
						<td style="width:218px;text-align: right;padding-right: 20px;"><span>安全类型</span></td>
						<td style="width:218px">
						    <select class="selectinp" id="apcli_mode" name="apcli_mode" onchange="show_sub(this.options[this.options.selectedIndex].value)">
								<option value='OPEN'>NONE</option>
								<option value='WEPAUTO' style="display:none;">WEP</option>
								<option value='WPAPSK'>WPA</option>
								<option value='WPA2PSK'>WPA2</option>
								<option value='WPA1PSKWPA2PSK'>WPAWPA2</option>
						  	</select>
						</td>
						<td>&nbsp;</td>
						<td>&nbsp;</td>
							</tr>
							<tr id="apcli_enc" style="display:block;">
								<td style="width:218px;text-align: right;padding-right: 20px;"><span>加密算法</span></td>
								<td style="width:218px" colspan="3">
									<input class="radiobox" type="radio" name="apcli_enc" value="TKIPAES"/>TKIP&nbsp;
									<input class="radiobox" type="radio" name="apcli_enc" value="TKIPAES"/>AES&nbsp;
									<input class="radiobox" type="radio" name="apcli_enc" value="TKIPAES"/>TKIP/AES&nbsp;
								</td>
							</tr>
							<tr id="wirelesskey" style="display:block;">
								<td style="width:218px;text-align: right;padding-right: 20px;"><span>无线密码</span></td>
								<td id="passortext"><input class="inp1" type="text" name="apcli_wpapsk" id="apcli_wpapsk" value=""></td>
								
							</tr>
						</table>
						<div class="subpart">
							<ul style="margin-left: -180px;">
								<li><b class="reselect" onClick="javascript:back_step();">上一步</b></li>
								<li><b class="connect" id="connect" onClick="javascript:wanmodeSetting_apply(this);">连接</b></li>
							</ul>
						</div>
					</div>
					<div class="subpart" style="display:none;padding-left:150px" id="submit1">
						<ul>
							<li><b class="cancel" onclick="javascript:parent.TINY.box.hide();">取消</b></li>
							<li><b id="opmodeSaveBtn" onclick="javascript:wanmodeSetting_apply(this);">保存</b></li>
						</ul>
					</div>
			
			</div>
		</div>
	</form>
</body>
</html>
