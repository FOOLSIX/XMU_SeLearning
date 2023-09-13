import java.security.SecureRandom;
import java.util.Scanner;

public class CAI3 {
    private static final SecureRandom rnd = new SecureRandom(); // 随机数生成器
    private static final Scanner scan = new Scanner(System.in);
    private static int sum = 0;
    private static double right = 0;
    private static int give_question() // 出题函数，返回答案
    {
        int a = 1 + rnd.nextInt(10); // 随机生成两个1~10之间的整数
        int b = 1 + rnd.nextInt(10);
        System.out.printf("How much is %d times %d?\n",a, b);
        return a * b;
    }

    private static void correct(int n) // 针对回答正确的评语
    {
        switch (n) {
            case 1 -> System.out.println("Very Good!");
            case 2 -> System.out.println("Excellent!");
            case 3 -> System.out.println("Nice work!");
            case 4 -> System.out.println("Keep up the good work!");
        }
    }

    private static void wrong(int n) // 针对回答错误的评语
    {
        switch (n) {
            case 1 -> System.out.println("No. Please try again.");
            case 2 -> System.out.println("Wrong. Try once more.");
            case 3 -> System.out.println("Don't give up!");
            case 4 -> System.out.println("No. Keep trying.");
        }
    }

    public static void main(String[] args)
    {
        System.out.println("This is CAI system ver3.0, input \"-1\" to quit.");
        while(true)
        {
            System.out.println("Start test.");
            int input = 0;
            while(sum<10)
            {
                int answer = give_question();
                input = scan.nextInt();
                if(input == answer)
                {
                    correct(1 + rnd.nextInt(4));
                    right++;
                    sum++;
                }
                else if(input == -1)
                    break;
                else {
                    do
                    {
                        sum++;
                        if(sum >= 10)
                            break;
                        wrong(1 + rnd.nextInt(4));
                        input = scan.nextInt();
                    }
                    while(input != answer);
                    if(sum >=10)
                        break;
                    correct(1 + rnd.nextInt(4));
                }
            }
            if(input == -1)
                break;
            double rate = right/(double)sum;
            System.out.printf("Your correct rate is %.2f\n", rate);

            if(rate>=0.75)
                System.out.println("Congratulations, you are ready to go to the next level!");
            else
                System.out.println("Please ask your teacher for extra help.");

            sum = 0;
            right = 0;
        }
        System.out.println("See you next time!");
    }
}
