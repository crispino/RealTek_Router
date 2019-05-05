<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<meta http-equiv="X-UA-Compatible" content="IE=8">
<meta http-equiv="Expires" content="0">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Cache-control" content="no-cache">
<meta http-equiv="Cache" content="no-cache">
<title>路由模式设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript" src="js/tinybox.js"></script>
<style type="text/css">
<!--
body{ color:#676767;}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:520px; width:670px; overflow:hidden; background:#fcfcfa url(images/fullbg.gif) repeat-x 0 -2px; padding:5px 15px;}
.int h1{ font-size:18px; line-height:50px; }
.int h1 span{ font-size:12px; color:#919191; padding-left:15px; font-weight:normal;}
.wirelesslistright{ height:330px;}
td.head {
  width: 35%;
  font-size: 9pt; 
  font-family: Arial, Helvetica, sans-serif;
  background-color: #E8F8FF;
  white-space: nowrap;
  color: #333333
}
td.head1 {
  width: 10%;
  font-size: 9pt; 
  font-family: Arial, Helvetica, sans-serif;
  background-color: #E8F8FF;
  white-space: nowrap;
  color: #333333
}

select {
  border: solid 1px #000;
  appearance:none;
  -moz-appearance:none;
  -webkit-appearance:none;
}
/*清除ie的默认选择框样式清除，隐藏下拉箭头*/
.inp3{ height:26px; border:1px #d2d2d2 solid; font-family:"微软雅黑"; color:#666; padding:0 5px;}
.selectinp_custom1{height:28px; border:1px #d2d2d2 solid; width:85px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 5px; background:#fff url(images/dropd.png) no-repeat right center;}
.selectinp_custom2{height:28px; border:1px #d2d2d2 solid; width:71px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 5px; background:#fff url(images/dropd.png) no-repeat right center;}
select::-ms-expand { display: none; }
.selectinp{height:28px; border:1px #d2d2d2 solid; width:198px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 10px; background:#fff url(images/dropd.png) no-repeat right center;}
.cfg{ padding:15px 0; height:330px;}
.cfgleft{ float:left; width:185px; height:330px; background:url(images/cfgleftbg.png) repeat-y;}
.cfgleft ul{}
.cfgleft li{ cursor:pointer; height:40px; line-height:40px; font-size:14px; color:#666; border-left:4px #dfdfdf solid ; padding-left:12px;}
.cfgleft li:hover{ border-left:4px #1e9ec5 solid;  font-weight:bold;}
.cfgleft li.on{ border-left:4px #1e9ec5 solid; color:#1e9ec5; font-weight:bold; font-size:16px; background:#fff url(images/arrow1.png) no-repeat 170px center;}
.cfgright{padding-left:30px; height:330px;}
.cfgright td{ height:36px; font-size:14px;}
.cfgright td span{ padding-left:100px; font-size:14px;}
.cfgright .spheight{padding-left:100px; height:60px; }
.cfgright td label{ font-size:14px; line-height:60px;}
.radiobox{ margin-top:-2px;}
.inp1 {text-indent: 12px;height:26px; border:1px #d2d2d2 solid; width:196px;font-family:"微软雅黑"; color:#666; padding:0;}
.subpart{ height:48px; padding-left:100px; padding-top:25px;}
.subpart ul{}
.subpart li{ float:left; padding-right:10px;}
.subpart b{ display:block; height:48px; cursor:pointer; width:170px; font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:48px;}
.subpart b:hover{ background:#56bbcb;}
.subpart .cancel{ width:110px; background:#99e4f0; }
-->
</style>

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
//	console.log($("#heUiopmode").serialize());
	if (document.heUiopmode.opMode[2].checked == true)
	{
//		Alert("正在保存设置，请稍候", 15); 
	  $('#connect').html('连接中...')
		parent.showChangeRtOpmode();
		
		document.getElementById("apcliEnbl").value="1";
		document.getElementById("natEnbl").value="0"; 
		
		$.ajax({
			type: "post",
			url: "/boafrm/heUiOpMode",
			data: $("#heUiopmode").serialize(),
			success: function(data){
				//alert("保存成功！");
			        setTimeout("reload_main()",1000);
			},
			error: function(data) {
				//alert("保存失败！\n" + data);
			}
		})
	}
	else
	{
		$("#opmodeSaveBtn").html("正在保存中...");
//		Alert("正在保存设置，请稍候", 15); 
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
				} else {

					apcli_enc_select[1].checked = true;
				}

			} else if(security.indexOf("WPA2") != -1 ) {
				security_select.value = "WPA2PSK";
				show_sub("WPA2PSK");
				
				if(security.indexOf("TKIP") != -1 && security.indexOf("AES") == -1) {
					apcli_enc_select[0].checked = true;
				} else {

					apcli_enc_select[1].checked = true;
				}

				
			}

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

<div class="int">
	<h1>路由模式设置<span>*请选择您的路由模式</span></h1>
    <div class="cfg">
        <div class="cfgright">
		   <form method="post" name="heUiopmode" id="heUiopmode" action="/boafrm/heUiOpMode">
           <table width="100%" border="0" cellspacing="0" cellpadding="0" >
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
	        <div class="wirelesslistright" style="overflow-x:auto;overflow-y:auto;" id="wirelesslistdiv">					
				<table id="wirelesslist"  width="610" border="1" cellpadding="2" cellspacing="1" height="400">
				<tr> 
					   <td class="head1" >信道</td>					   		
					   <td class="head" >SSID</td>
					   <td class="head" >安全类型</td>
					   <td class="head" >信号强度(%)</td>
								<td class="head">选择</td>
							    <td style="display:none"></td>
				</tr>
					<% draw_wlSiteSurveyTbl(); %>   					
				</table>
			</div>
            <div class="subpart" id="rescan" style="padding:0;">
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
								<td style="width:218px"><span>网络名称</span></td>
				        <td style="width:218px"><input class="inp1" name="apcli_ssid" id="apcli_ssid" value="" readonly="readonly"</td>
							</tr>
							<tr id="authType" style="display:block;">
						<td style="width:218px"><span>安全类型</span></td>
						<td style="width:218px">
						    <select class="selectinp" id="apcli_mode" name="apcli_mode" onchange="show_sub(this.options[this.options.selectedIndex].value)">
								<option value='OPEN'>NONE</option>
								<option value='WEP' >WEP</option>
								<option value='WPAPSK'>WPA</option>
								<option value='WPA2PSK'>WPA2</option>
						  	</select>
						</td>
						<td>&nbsp;</td>
						<td>&nbsp;</td>
							</tr>
							<tr id="apcli_enc" style="display:block;">
						<td style="width:218px"><span>加密算法</span></td>
						<td style="width:218px" colspan="3">
							<input class="radiobox" type="radio" name="apcli_enc" value="TKIP"/>TKIP&nbsp;
									<input class="radiobox" type="radio" name="apcli_enc" value="AES"/>AES&nbsp;
								</td>
							</tr>
							<tr id="wirelesskey" style="display:block;">
								<td style="width:218px"><span>无线密码</span></td>
								<td id="passortext"><input class="inp1" type="text" name="apcli_wpapsk" id="apcli_wpapsk" value=""></td>
								
							</tr>
						</table>
						<div class="subpart">
							<ul>
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
		</div>
	</form>
</body>
</html>
