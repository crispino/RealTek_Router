<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>工作模式</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript" src="js/jquery.js"></script>
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
		
		Alert("正在保存设置，请稍候", 30); 
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
<div class="int int_1">
	<h1>工作模式<span>*请选择您的工作模式</span></h1>
    <div class="cfg">
     
      	<form method="post" name="heUiopmode" id="heUiopmode" action="/boafrm/heUiOpMode">
            <table width="100%" border="0" cellspacing="0" cellpadding="0" style="padding-left: 50px;">
            	<tr>
                <td width ="35%" valign="top"><input type="radio" value="0" name="opMode" onClick="switch_conType()" ></input>
                <font size=2 style="font-size:14px;">  路由模式</font>
				</td>
				<td width ="35%" valign="top"><input type="radio" value="1" name="opMode" onClick="switch_conType()" ></input>
                <font size=2 style="font-size:14px;">  桥接模式</font>
				</td>
				<td width ="30%" valign="top"><input type="radio" value="3" name="opMode" onClick="switch_conType()" ></input>
                <font size=2 style="font-size:14px;">  中继模式</font>
                </td>
                </tr>

            </table>
	        <div id="apcli2or5">
	      		<table width="100%" border="0" cellspacing="0" cellpadding="0">
	      			<tr>
		        		<td width="40%" ><span style="font-size:16px;padding-left: 134px;">当前连接的SSID:</span></td>
			        	<td><span style="font-size:16px;padding: 0;"><% getHeInfo("repeaterConnectSsid"); %></span></td>	
							</tr>
		        	<tr>
		        		<td><span style="font-size:16px;padding-left: 134px;">当前连接状态:</span></td>
			        	<td><span style="font-size:16px;padding: 0;"><% getHeInfo("repeaterConnectState"); %></span></td>	
							</tr>
	      		</table>
	        </div>
            <div class="subpart">
                <ul>
                	<li><b class="save" id="opmodeSaveBtn" name="opmodeSaveBtn">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
        </form>
      
    </div>
</div>
</body>
</html>
