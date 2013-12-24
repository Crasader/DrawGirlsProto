
	var delegator2 = [];
	var delekeyCnt2 = 1;
	var addDelegate2 = function(obj){
		delegator2[delekeyCnt2]=obj;
		delekeyCnt2++;
		return delekeyCnt2-1;
	}
	

		
	var imageSelector = function(value,option){
		var img;
		
		if(value)img = "src='../images/"+value+"'";
		else img=""
		
		if(value)_val = value;
		else _val="";
		
		var op="<input type='image' value='"+_val+"' "+img+" class='imageSelector LQEditor' width=100>";
		return op;
	}
	
	var resourceSelector = function(value,option){	
		if(value)_val = value;
		else _val="";
		var getsize = "true";
		//if(option["getSize"])getsize="true";
		
		var op="<input type='text' value='"+_val+"' getSize='"+getsize+"' class='resourceSelector LQEditor' width=100><br><input type='button' value='select' class='resourceSelectorBtn'>";
		return op;
	}
	
	var imageViewer = function(value,option){
		var img;
		
		if(value)img = "<img src='../images/"+value+"' width=100>";
		else img=""
		
		return img;
	}
	
	window.changeJsonFormInput = function(input,value,size) {
		log("changeJsonFormInput:"+input+",value:"+value+",size:"+size);
		var obj = delegator2[input];
		
		
		obj.attr("src","../images/"+value);
		obj.attr("value",value);
		obj.val(value);
		
		obj.parent().parent().parent().find("td[field=size]").children("input").val(parseInt(size))
		
		
		
		//$(input).val(value).change();
	}

	window.changeJsonFormInputByResource = function(input,value,size) {
		log("changeJsonFormInput:"+input+",value:"+value+",size:"+size);
		var obj = delegator2[input];
		
		
		obj.attr("src","../images/"+value);
		obj.attr("value",value);
		obj.val(value);
		
		if(obj.attr("getSize")=="true"){
			obj.parent().parent().parent().find("td[field=size]").children("input").val(parseInt(size))
		}
		
		
		//$(input).val(value).change();
	}

	$(document).ready(function(){
		$('body').on('click','.imageSelector',function(){
			var delkey = addDelegate2($(this));
			window.open('./admin_images.php?delkey='+delkey,'imageselector','width=1000 height=800 menubar=no status=no');
		});
		
		$('body').on('click','.resourceSelectorBtn',function(){
			var obj = $(this).parent().children(".LQEditor");
			var delkey = addDelegate2(obj);
			window.open('./admin_resources.php?delkey='+delkey,'resourceselector','width=1000 height=800 menubar=no status=no');
		});
	});