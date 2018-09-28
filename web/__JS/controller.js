function gatherTireTable(table, tieraInstance) {
    var noS = table.getElementsByClassName("noS");
    var i = 0;
    var rs = [];
    for (; i < noS.length; i++) {
        var j = 0;

        var name = noS[i].getElementsByTagName('label')[0].innerHTML;
        var value = noS[i].getElementsByTagName('input')[0].value;

        for (; j < dataS.tierProperties.length; j++) {
            if (dataS.tierProperties[j].name == name) {
                rs[rs.length] = [dataS.tierProperties[j].value, value];
            }
        }
    }
    var bu = table.getElementsByClassName('rowID');

    i = 0;
    for (; i < bu.length; i++) {
        var j = 0;
        var sele = bu[i].getElementsByClassName('selectC')[0];
        var name = sele.options[sele.selectedIndex].text;
        var value = bu[i].getElementsByTagName('input')[0].value;
        // console.log(name,value);
        for (; j < dataS.tierProperties.length; j++) {
            //console.log(dataS.tierProperties[j].name, name);
            if (dataS.tierProperties[j].name == name) {

                rs[rs.length] = [dataS.tierProperties[j].value, value];
            }
        }
    }
    tieraInstance.addATier(rs);
    // console.log("call");

}
function gather() {
    var wiera = new Wiera();
    var regions = document.getElementsByClassName("regionalBox")[0].children;
    var wieraId = document.getElementById("wieraID").value;
    var wieraDesc = document.getElementById("wieraDesc").value;
    wiera.configuration(wieraId, wieraDesc);
    var i = 0;
    var len = "regionalContent".length;

    for (; i < regions.length; i++) {
        if (regions[i].className == "regionalContent") {
            var num = parseInt(regions[i].id.slice(len, regions[i].id.length));
            //==========
            if (!num) {
                alert(regions[i].id);
            } else {

            }



            var region = getTheFirstEle(regions[i], [["t", "p"], ["c", "regionName"]]);
            var tieraInstance = wiera.addARegionalTiera(region.value);
            // console.log(wiera);
            var tieraName = getTheFirstEle(regions[i], [['t', 'div'], ['t', 'p'], ['c', 'tieraName']]).value;
            tieraInstance.setName(tieraName);

            var tables = getTheFirstEle(regions[i], [['t', 'div'], ['t', 'div']]).children;
            var countI = 0;
            for (; countI < tables.length; countI++) {
                //
                ///console.log(tables.length);
                if (tables[countI].className != "tierAddButton") {
                    var table = getTheFirstEle(tables[countI], [['c', 'tierInfoTable']]);
                    gatherTireTable(table, tieraInstance);
                }
            }
        }
    }

    printTo(syntaxHighlight(JSON.stringify(wiera.wieraPolicy, null, 2)));
}
function getTheFirstChildWithClassName(ele, className) {
    var i = 0;
    var children = ele.children;

    for (; i < children.length; i++) {
        if (children[i].className == className) {
            return children[i];
        }
    }
}
function getTheFirstChildWithTagName(ele, tagName) {
    var i = 0;
    var children = ele.getElementsByTagName(tagName);
    if (children.length > 0) {
        return children[0];
    } else {
        return null;
    }

}

function getTheFirstEle(ele, list) {
    //[ ['c', ''], ['t', '']]
    var e = 0;
    var l = 0;
    var temp = ele;
    for (; l < list.length; l++) {
        if (list[l][0] == "c") {
            temp = getTheFirstChildWithClassName(temp, list[l][1]);
        } else {
            temp = getTheFirstChildWithTagName(temp, list[l][1]);
        }
    }
    return temp;

}

