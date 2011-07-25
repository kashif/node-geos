var assert = require("assert");
var Geometry = require("../index").Geometry;
var StrTree = require("../index").StrTree;
var g = new Geometry("POINT(10 10)");
var t = new StrTree();
try {
    new StrTree(1);
} catch (e) {
    console.log(e);
}
t = new StrTree(10);

console.log(g.toWkt());
var object = {
    test: "testing"
};
try {
    console.log("t.insert: " + t.insert(g,"testing"));
} catch(e) {
    console.log("bla... " + e);
}
/*try {
    console.log("t.remove(g): " + t.remove(g));
} catch(e) {
    console.log(e);
}*/
try {
    console.log(t.query(g,function(data) {
        console.log("found the geom but to stupid to get the item ;)");
    }));
} catch(e) {
    console.log(e);

}
//console.log("t.remove(g): " + t.remove(g));
//console.log(t.insert(g));
/*try {
    t.remove();
} catch (ev) {
    console.log(ev);
}*/
//console.log(t.remove(g));
//console.log(t.remove(g));
