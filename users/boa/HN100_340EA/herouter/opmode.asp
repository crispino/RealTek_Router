<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>工作模式</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript" src="js/jquery.js"></script>
<style type="text/css">
<!--
body{ color:#676767;}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:520px; width:670px; overflow:hidden; background:#fcfcfa url(images/fullbg.gif) repeat-x 0 -2px; padding:5px 15px;}
.int h1{ font-size:18px; line-height:50px; border-bottom:1px #d3d3d3 solid;}
.int h1 span{ font-size:12px; color:#919191; padding-left:15px; font-weight:normal;}
.mainsetting{ padding:30px; }
.mainsetting h2{ font-size:14px; line-height:38px;}
.c1{ height:48px;}
.c1 b{ display:block; height:48px; cursor:pointer; width:190px; font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:48px;}
.c1 a{ background:url(images/help.png) no-repeat center center; display:block; float:left; height:48px; width:30px;}
.c1 b:hover{ background:#56bbcb;}
.c1 span{  font-size:12px; color:#919191; padding:0 0 0 15px; float:left; font-weight:normal; line-height:48px;}
.c1 .err{ background:#e71b1b;}
.c1 .err:hover{ background:#da1515;}
.mainsetting h3{ font-size:24px; font-weight:normal; line-height:50px; color:#29b8e4;}
.c2{ border-top:1px #e6e6e6 solid; margin-top:80px; line-height:30px;}

.selectinp{height:28px; border:1px #d2d2d2 solid; width:230px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 10px; background:#fff url(images/dropd.png) no-repeat right center;}

.wirelesslistright{ height:330px;}
td.head {
  width: 40%;
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
.cfg{ padding:15px 0; height:330px;}
.cfgleft{ float:left; width:185px; height:330px; background:url(images/cfgleftbg.png) repeat-y;}
.cfgleft ul{}
.cfgleft li{ cursor:pointer; height:40px; line-height:40px; font-size:14px; color:#666; border-left:4px #dfdfdf solid ; padding-left:12px;}
.cfgleft li:hover{ border-left:4px #1e9ec5 solid;  font-weight:bold;}
.cfgleft li.on{ border-left:4px #1e9ec5 solid; color:#1e9ec5; font-weight:bold; font-size:16px; background:#fff url(images/arrow1.png) no-repeat 170px center;}
.cfgright{padding-left:100px; height:330px;}
.cfgright td{ height:36px; font-size:14px;}
.cfgright td span{ padding-left:100px; font-size:14px;}
.cfgright .spheight{padding-left:100px; height:60px; }
.cfgright td label{ font-size:14px; line-height:60px;}
.radiobox{ margin-top:-2px;}
.inp1 {height:26px; border:1px #d2d2d2 solid; width:160px;font-family:"微软雅黑"; color:#666; padding:0 10px;}
.selectinp_custom2{height:28px; border:1px #d2d2d2 solid; width:71px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 5px; background:#fff url(images/dropd.png) no-repeat right center;}
.subpart{ height:48px; padding-left:120px; padding-top:25px;}
.subpart1{ height:48px; padding-left:120px; padding-top:80px;}
.subpart ul{}
.subpart li{ float:left; padding-right:10px;}
.subpart b{ display:block; height:48px; cursor:pointer; width:170px; font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:48px;}
.subpart b:hover{ background:#56bbcb;}
.subpart .cancel{ width:110px; background:#99e4f0; }
.STYLE1 {
	color: #FF0000;
	font-size: 10px;
}
select {
  border: solid 1px #000;
  appearance:none;
  -moz-appearance:none;
  -webkit-appearance:none;
}
/*清除ie的默认选择框样式清除，隐藏下拉箭头*/
select::-ms-expand { display: none; }
-->
</style>
</head>

<script language="JavaScript" type="text/javascript">

function switch_conType(){
	if(document.heUiopmode.opMode[2].checked)
	{
		 document.getElementById("apcli2or5").style.display="block";
	}
	else
		{
		  document.getElementById("apcli2or5").style.display="none";
		}
}

$(document).ready(function(){
	var opmode=<% getIndex("opMode"); %>;
	var repeaterMode = <% getHeInfo("repeaterMode"); %>; 
	
	if(opmode==0){
		$("input[name='opMode']")[0].checked=true;
	}else if(opmode==1 && repeaterMode==0){
		$("input[name='opMode']")[1].checked=true;
	}else if(opmode==1 && repeaterMode==1){
		$("input[name='opMode']")[2].checked=true;
	}
	$("#opmodeSaveBtn").click(function(){
		$("#opmodeSaveBtn").html("正在保存中...");
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
  
    })
});
function initValue(){
  var opmode=<% getIndex("opMode"); %>;
  var repeaterMode = <% getHeInfo("repeaterMode"); %>; 

  if(opmode==0){
      $("input[name='opMode']")[0].checked=true;  
	  document.getElementById("apcli2or5").style.display="none";
  }else if(opmode==1 && repeaterMode!=1){ 
  	  $("input[name='opMode']")[1].checked=true;
	  document.getElementById("apcli2or5").style.display="none";
  }else if(opmode==1 && repeaterMode==1){ 
  	  $("input[name='opMode']")[2].checked=true;
	  document.getElementById("apcli2or5").style.display="block";
  }
  
  return true;
}
</script>

<body onLoad="initValue()">
<div class="int">
	<h1>工作模式<span>*请选择您的工作模式</span></h1>
    <div class="cfg">
      <div class="cfgright">
      	<form method="post" name="heUiopmode" id="heUiopmode" action="/boafrm/heUiOpMode">
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
            	<tr>
                <td width ="23%" valign="top"><input type="radio" value="0" name="opMode" onClick="switch_conType()" ></input>
                <font size=2 style="font-size:14px;">  路由模式</font>
				</td>
				<td width ="23%" valign="top"><input type="radio" value="1" name="opMode" onClick="switch_conType()" ></input>
                <font size=2 style="font-size:14px;">  桥接模式</font>
				</td>
				<td width ="23%" valign="top"><input type="radio" value="3" name="opMode" onClick="switch_conType()" ></input>
                <font size=2 style="font-size:14px;">  中继模式</font>
                </td>
                </tr>

            </table>
	        <div style="display: block;padding-left:150px" id="apcli2or5">
	        <!--	
					<script language="JavaScript" type="text/javascript">
					if(wlan_num>1){
						document.write("<tr><br></br>");						
		        		document.write("<td><span style=\"font-size:16px\">无线选择:</span></td>");	
		        		document.write("<select class=\"selectinp_custom2\" name=\"wirelesssel\" id=\"wirelesssel\">");	
						document.write("<option value =\"0\">2.4G</option>");
						document.write("<option value =\"1\">5.0G</option>");
						document.write("</select>");	
						document.write("</tr>");					
					}							
					</script>
			-->				
	        	<!--<tr>
	        		<br></br>
	        		<td><span style="font-size:16px">当前连接的SSID:</span></td>
		        	<td><span style="font-size:16px"><% getHeInfo("repeaterConnectSsid"); %></span></td>	
						</tr>
	        	<tr>
	        		<br></br>
	        		<td><span style="font-size:16px">当前连接状态:</span></td>
		        	<td><span style="font-size:16px"><% getHeInfo("repeaterConnectState"); %></span></td>	
						</tr>				-->
	        </div>
            <div class="subpart">
                <ul>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                    <li><b class="save" id="opmodeSaveBtn" name="opmodeSaveBtn">保存</b></li>
                </ul>
            </div>
        </form>
      </div>
    </div>
</div>
</body>
</html>
