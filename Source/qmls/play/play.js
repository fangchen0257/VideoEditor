
function parseWidthHeightProportion(strVal)
{
    var proportionArr = strVal.split(":")
    return  [parseInt(proportionArr[0]), parseInt(proportionArr[1])]
}
