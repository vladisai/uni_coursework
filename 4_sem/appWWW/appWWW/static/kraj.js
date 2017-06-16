function initSockets() {
    socket = new WebSocket("ws://" + window.location.host + "/");
    socket.onmessage = function(e) {
        requestDataFromServer();
    }
    if (socket.readyState == WebSocket.OPEN) socket.onopen();
}

var title = 'Cały kraj'
var wojewodztwo_set_data = null;
var kraj_data = null;
var path = window.location.pathname;


function checkAndFillWojewodztwoSetData() {
    if (wojewodztwo_set_data) {
        var context = {'unit': 'Województwo', 'data': wojewodztwo_set_data};
        var html = stats_table_template.render(context);
        $('#wojewodztwo_table_container').html(html);
    }
}

function checkAndFillKrajData() {
    if (kraj_data) {
        var context = {'data' : kraj_data};
        var html = general_stats_template.render(context);
        $('#general_stats_container').html(html);
    }
}

function requestDataFromServer() {
    $.getJSON("api/wojewodztwo", function(data) {
        wojewodztwo_set_data = data;
        if (wojewodztwo_set_data) {
            localStorage.setItem(path + '_wojewodztwo_set', JSON.stringify(wojewodztwo_set_data));
        }
        checkAndFillWojewodztwoSetData();
    });
    $.getJSON("api/kraj/", function(data) {
        kraj_data = data[0];
        if (kraj_data) {
            localStorage.setItem(path + '_kraj', JSON.stringify(kraj_data));
        }
        checkAndFillKrajData();
    });
}

function checkAndFillAllData() {
    checkAndFillKrajData();
    checkAndFillWojewodztwoSetData();
}

function restoreCache() {
    wojewodztwo_set_data = JSON.parse(localStorage.getItem(path + '_wojewodztwo_set', {}));
    kraj_data = JSON.parse(localStorage.getItem(path + '_kraj', {}));
}

$(function() {
    var context = {title: title};
    var html    = header_template.render(context);
    $('#header_container').html(html);
    restoreCache();
    checkAndFillAllData();
    requestDataFromServer();
    initSockets();
});
