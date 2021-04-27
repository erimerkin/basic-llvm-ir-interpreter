void parseParanExpr(){
    string token = getToken();

    if (token.value == '('){
        parseExpr();
        token = getToken();
        if (token.value != ')'){
            syntaxError(line);
        }

        return x;
    }

    if (token.type == identifier){

    }

    if (token.type == number){

    } else {
        syntaxError(line);
    }

}

void parseFactor(){

}

void parseMorefactor(){

}

void parseTerm(){


}

void parseChoose(){

}

void parsePrint(){
    
}

void parseMoreterms(){

}

void parseExpr(){
    
}

