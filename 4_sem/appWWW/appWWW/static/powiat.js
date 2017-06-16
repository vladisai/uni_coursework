var title = "Powiat {{ title }}";
var pk = {{ pk }};
var powiat_data = null;
var gmina_set_data = null;
var path = window.location.pathname;

function checkAndFillGminaSetData() {
    if (gmina_set_data) {
        var context = {'unit': 'Powiat', 'data': gmina_set_data};
        var html = stats_table_template.render(context);
        $('#powiat_table_container').html(html);
    }
}

function checkAndFillPowiatData() {
    if (powiat_data) {
        var context = {'data' : powiat_data};
        var html = general_stats_template.render(context);
        $('#general_stats_container').html(html);
    }
}

function requestDataFromServer() {
    $.getJSON("/api/powiat/" + pk, function(data) {
        powiat_data = data;
        localStorage.setItem(path + '_powiat', JSON.stringify(powiat_data));
        checkAndFillPowiatData();
    });
    $.getJSON("/api/powiat/" + pk + "/gmina_set", function(data) {
        gmina_set_data = data;
        localStorage.setItem(path + '_gmina_set', JSON.stringify(gmina_set_data));
        checkAndFillGminaSetData();
    });
}

function checkAndFillAllData() {
    checkAndFillPowiatData();
    checkAndFillGminaSetData();
}

function restoreCache() {
    gmina_set_data = JSON.parse(localStorage.getItem(path + '_gmina_set', null));
    powiat_data = JSON.parse(localStorage.getItem(path + '_powiat', null));
}

$(function() {
    var context = {title: title};
    var html    = header_template.render(context);
    $('#header_container').html(html);
    restoreCache();
    checkAndFillAllData();
    requestDataFromServer();
});
