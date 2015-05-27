

API = {



    post:function (url, param, callback, err) {
        $.ajax({
            type:"POST",
            url:url,
            dataType: 'json',
            data:param,
            timeout:30000,
            success:callback,
            error: err
        });
    }
};
