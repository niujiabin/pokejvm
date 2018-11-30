
public class Test{

   String name;
   int age;

   int getAge(){
   return age;
   }

   void setAge(int age){
   this.age=age;
   }

   public static void main(String args[]){

   Test test = new Test();
   test.setAge(10);
   System.out.print(test.getAge());
   }
}