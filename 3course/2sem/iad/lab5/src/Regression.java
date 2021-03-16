import weka.core.Instance;
import weka.core.Instances;
import weka.core.converters.ConverterUtils.DataSource;
import weka.classifiers.functions.SimpleLinearRegression;

public class Regression {
    public static void main(String args[]) throws Exception{

        DataSource source = new DataSource("./house.arff");
        Instances dataset = source.getDataSet();
        dataset.setClassIndex(dataset.numAttributes()-1);

        // Побудова моделі
        SimpleLinearRegression model = new SimpleLinearRegression();
        model.buildClassifier(dataset);
        System.out.println("LR FORMULA : Simple "+model);

        // Вгадуємо ціну
        Instance myHouse = dataset.lastInstance();
        double price = model.classifyInstance(myHouse);
        System.out.println("-------------------------");
        System.out.println("PRECTING THE PRICE : "+price);
    }
}
