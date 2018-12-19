class Main {

    static int sum(int a,int b,int c) {
    int result;
    result=(a+b)*c*b;
    System.out.println(result);
    return result;
    
}

public static void main(String[] args) {
    int first;
    String message;
    int second;
    int k=3;
    String s="str";
    double d;
    double o;
    int rez;
    first=14.5;
    second=2;
    message="Hello World";
    String stroka="opa f5";
    int q=message.length();
    String sb;
    sb=message.substring(1,2);
    rez=sum(k,first,second);
    d=45;
    System.out.println(d);
    o=Math.pow(second,first);
    if(k != 14) {
    System.out.println("k ne ravno 14");
    
}
System.out.println(o);
    System.out.println(message);
    System.out.println(rez);
    System.out.println(k);
    System.out.println(s);
    System.out.println(first);
    
}

}