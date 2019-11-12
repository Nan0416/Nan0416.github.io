function Animal(weight, age){
    this.weight = weight;
    this.age = age;
}
Animal.prototype.word = "q";

let animal = new Animal(10, 12);
let cat = Object.create(animal);
Animal.prototype.jump = function(){
    return "jump";
}
cat.word = "c";
console.log(animal.word);
console.log(cat.word);
console.log(cat.jump());