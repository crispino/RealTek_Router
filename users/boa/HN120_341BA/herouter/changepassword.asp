<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>密码修改</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
</head>
<script language="JavaScript" type="text/javascript">

var oldpass_error="-1";
function save()
{
    var oldpass = document.Adm.oldpass.value;
	var newpass = document.Adm.newpass.value;
	var admpass = document.Adm.admpass.value;
	var len = newpass.length;
	
	if(oldpass == ''){
		alert('请输入原密码');
		return false;
	}
	
	if (len < 8 || len > 15) 
	{
		document.getElementById("lenerr").style.display = "block";
		document.getElementById("differr").style.display = "none";
		document.getElementById("buferr").style.display = "none";
		return false;
	}
	else
	{
		document.getElementById("lenerr").style.display = "none";	
	}
	
	var reg =/\s/;

  if(reg.test(newpass))
  {
		document.getElementById("buferr").style.display = "block";
		return false;
  }
  else
  {
    document.getElementById("buferr").style.display = "none";	
  }
  
  var regplus = /(?=.*[a-z])(?=.*\d)(?=.*[$#@!~%^&*])[a-z\d$#@!~%^&*]{8,15}/i;
	if(!regplus.test(newpass))
  {
		document.getElementById("comperr").style.display = "block";
		return false;
  }

	if(newpass == oldpass)
	{
	    document.getElementById("sameerr").style.display = "block";
		document.getElementById("lenerr").style.display = "none";
		document.getElementById("buferr").style.display = "none";
		return false;
	}
	else
	{
		document.getElementById("sameerr").style.display = "none";
	}
	if (admpass != newpass) 
	{
		document.getElementById("differr").style.display = "block";
		document.getElementById("buferr").style.display = "none";
		return false;
	}
	else
	{
		document.getElementById("differr").style.display = "none";	
	}


  
  
	document.getElementById("Adm").submit();
	
	return true;
}
function admInit()
{
	oldpass_error=GetQueryString("oldpass_error");
	if(oldpass_error=="1"){
		document.getElementById("defaultpwd").style.visibility = "hidden";
		document.getElementById("defaultpwd").style.display = "none";
		document.getElementById("pwderr").style.display = "block";
		document.Adm.admuser.focus();
		return;
	}
	else if(oldpass_error=="0"){
		parent.showchangepwd();
	}
}
$(function(){
	$('#newpass').focus(function(){
		$('#showfocus').css('display','block');
		document.getElementById("sameerr").style.display = "none";
		document.getElementById("lenerr").style.display = "none";
		document.getElementById("buferr").style.display = "none";
		document.getElementById("comperr").style.display = "none";
	});
	$('#newpass').blur(function(){
		$('#showfocus').css('display','none');
	})
	$('#admpass').focus(function(){
		$('#differr').css('display','none');
	})
	
})
</script>

<body onLoad="admInit()">
<div class="int">
	<h1>修改和目路由管理界面登录密码</h1>
    <div class="cfg" >
      
      	<form method="post" name="Adm" id="Adm" action="/boafrm/setSysAdm">
            <table width="100%" border="0" cellspacing="0" cellpadding="0" style="padding-left: 70px;">
            	<tr id="user" style="display:none">
                <td><span>用户名</span></td>
                <td><input class="inp2" name="admuser" value="user"/></td>
              </tr>
              <tr>
                <td><span>现有密码</span></td>
                <td><input class="inp2" type="password" name="oldpass" value=""/></td>
                <td id="defaultpwd"><font color="#CF9E9E"><h5>(初始密码见盒底标签)</h5></font></td>
                <td><em id="pwderr" style="display:none;padding-left:18px;">密码错误，请重试</em></td>
              </tr>
              <tr>
                <td width="22%"><span>新密码</span></td>
                <td width="32%"><input id="newpass" class="inp2" type="password" name="newpass" value=""/></td>
                <td id="lenerr" style="display:none;line-height:36px;"><em style="padding-left:18px;font-size: 12px;">密码长度8-15位</em></td>
                <td id="buferr" style="display:none;line-height:36px;"><em style="padding-left:18px;font-size: 12px;">不能包含空格</em></td>
                <td id="sameerr" style="display:none;line-height:36px;"><em style="padding-left:18px;font-size: 12px;">不能和旧密码一样</em></td>
                <td id="comperr" style="display:none;line-height:36px;"><em style="padding-left:18px;font-size: 12px;">密码须同时包含字母数字特殊字符</em></td>
                <td id="showfocus" style="display:none;font-size: 12px;color: #CF9E9E;line-height: 16px;">长度8-15位<br />须同时包含字母数字特殊字符</td>
              </tr>
              <tr>
                <td><span>重复新密码</span></td>
                <td><input class="inp2" type="password" id="admpass" name="admpass" value=""/></td>
                <td><em id="differr" style="display:none;padding-left:18px;">请与新密码保持一致</em></td>
              </tr>
            </table>
            <div class="subpart" style="padding-left: 50px;">
                <ul>
                    <li><b class="save" onClick="save()">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
        </form>
      
    </div>
</div>
</body>
</html>
