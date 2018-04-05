var dataS;
var uiWorker;
var action;
var selectElementClass = "selectC";
var selectIDSuffix = "SelectOptional";
var tierInfoTableClass = "tierInfoTable";
var tierRowClass = "tierRow";
var regionalContentClass = "regionalContent";
var rowID = 1;
var regionNum = 2;
function parseTableId(str) {
    var len = str.length;
    var firstt = str.indexOf('t');
    var lastt = str.lastIndexOf('t');
    var firstT = str.indexOf('T');
    var regionNum = parseInt(str.slice(firstt + 1, lastt));
    var tierNum = parseInt(str.slice(lastt + 1, firstT));
    if (regionNum && tierNum) {
        var obj = new Object();
        obj.regionNum = regionNum;
        obj.tierNum = tierNum;
        return obj;
    }
    return null;
}
function getTableId(idList) {
    var i = 0;
    var max = 0;
    var region = 0;
    while (i < idList.length) {
        if (idList[i]) {
            region = idList[i].regionNum;
            if (max < idList[i].tierNum) {
                max = idList[i].tierNum;
            }
        }
        i += 1;
    }
    if (max == 0) {
        return null;
    }
    max = max + 1;

    return 't' + region + 't' + max + 'Table';
}
function dataSource() {

    this.tierProperties = [
        { name: "Tier name", value: "tier_name", value_type: "text", optional: false },
        { name: "Tier size", value: "tier_size", value_type: "text", optional: false },
        { name: "Tier type", value: "tier_type", value_type: "number", optional: false },
        { name: "Tier location", value: "tier_location", value_type: "text", optional: true },
        { name: "Tier expected latency", value: "tier_location", value_type: "number", optional: true },

    ];
}
function UIWorker() {
    this.drawTierInfoTable = function (data, class_, tableId, initial) {
        // draw 
        str = '<table class="' + class_ + '" id="' + tableId + '">';
        str += "<tbody>";
        var i = 0;
        for (; i < data.length; i++) {
            if (!data[i].optional) {
                var temp = '<tr class = "noS"><td><label>' + data[i].name + '</label></td><td><input type="' + data[i].value_type + '" class="' + tierRowClass + '"></input></td></tr>';
                str += temp;
            }
        }

        str += '<tr class="bu"><td><button type="button" onclick="action.addAnOptionalRow(dataS.tierProperties, selectElementClass,\'' + tableId + '\')">add a new property</button></td>';
        if (!initial) {
            // add remove button
            str += '<td><button type="button" onclick="action.removeATier(\'' + tableId + '\')">remove this tier</button></td>';

        }
        str += '</tr></tbody></table>';
        return str;
    }
    this.drawSelectProperties = function (data, class_) {
        str = '<select class = "' + class_ + '"><option value="new ...">New ... </option>';
        var i = 0;
        for (; i < data.length; i++) {
            if (data[i].optional) {
                str += '<option value="' + data[i].value + '">' + data[i].name + '</option>';
            }
        }
        str += "</select>";
        return str;
    }
    this.drawATieraUnder = function (div, num) {

        var id = "regionalContent" + num;
        div.setAttribute('class', regionalContentClass);
        div.setAttribute('Id', id);
        var str = '<p><label>Region</label><input type="text" class="regionName" id="regionName' + num + '"></input></p>';
        str += '<div class="tieraInfo">';
        str += '<p><label>Tiera name</label><input type="text" class="tieraName" id="tieraName' + num + '"></input></p>';
        str += '<div class="tiersInfo" id="t' + num + '">';
        str += '<div>';
        str += uiWorker.drawTierInfoTable(dataS.tierProperties, tierInfoTableClass, "t" + num + "t1Table", true);
        // console.log(uiWorker.drawTierInfoTable(dataS.tierProperties, tierInfoTableClass, "t"+ num + "t1Table", true));
        str += '</div>';
        str += '<div class = "tierAddButton"><button type="button" onclick="action.addATier(dataS.tierProperties, \'regionalContent' + num + '\')">Add a new tier</button></div>'
        str += "</div></div>";

        div.innerHTML = str;
    }
}
function Action() {
    this.addAnOptionalRow = function (data, selectClass, tableId) {
        // base on a table
        ;
        var table = document.getElementById(tableId);
        var len = table.rows.length;
        var row = table.insertRow(len - 1);
        row.setAttribute('id', "rowID" + rowID);
        row.setAttribute('class', 'rowID');
        var cell1 = row.insertCell(0);
        var cell2 = row.insertCell(1);
        var cell3 = row.insertCell(2);
        //console.log(data);
        cell1.innerHTML = "<label>" + uiWorker.drawSelectProperties(data, selectClass) + "</label>";
        cell2.innerHTML = '<input type="text" class="' + tierRowClass + '"></input>';
        cell3.innerHTML = '<button type="button" onclick="action.removeAnOptionalRow(\'rowID' + rowID + '\')">Remove</button>';

        rowID += 1;

    }
    this.removeAnOptionalRow = function (rowId) {
        
        var self = document.getElementById(rowId);
        var parent = document.getElementById(rowId).parentElement;
        parent.removeChild(self);
    }
    this.addATier = function (data, regionalID) {
        var tablesBox = document.getElementById(regionalID).getElementsByClassName("tieraInfo")[0].getElementsByClassName("tiersInfo")[0];
        var regionT = document.getElementById(regionalID);
        var beforePostion = tablesBox.getElementsByClassName("tierAddButton")[0];
        var tables = regionT.getElementsByClassName(tierInfoTableClass);
        var count = tables.length;
        var tablesId = new Array();
        var i = 0;
        while (i < count) {
            tablesId[i] = parseTableId(tables[i].id);
            i += 1;
        }
        var newTableId = getTableId(tablesId);
        this._addATier(data, tablesBox, beforePostion, newTableId);
    }
    this._addATier = function (data, under, before, id) {
        //console.log(under);
        var str = uiWorker.drawTierInfoTable(data, tierInfoTableClass, id, false);
        var newTable = document.createElement('div');
        newTable.innerHTML = str;
        under.insertBefore(newTable, before);

    }
    this.removeATier = function (tabelID) {

        var self = document.getElementById(tabelID);
        var parent = document.getElementById(tabelID).parentElement;
        var gparent = parent.parentElement;
        gparent.removeChild(parent);

    }
    this.addATiera = function () {

        var beforePostion = document.getElementById('addATieraButtonDiv');
        var container = beforePostion.parentElement;
        var newTiera = document.createElement('div');
        uiWorker.drawATieraUnder(newTiera, regionNum);
        container.insertBefore(newTiera, beforePostion);
        regionNum += 1;
    }


}

dataS = new dataSource();
uiWorker = new UIWorker();
action = new Action();
