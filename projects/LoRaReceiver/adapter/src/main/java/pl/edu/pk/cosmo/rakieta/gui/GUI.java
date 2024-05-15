package pl.edu.pk.cosmo.rakieta.gui;

import javax.swing.*;
import org.knowm.xchart.QuickChart;
import org.knowm.xchart.SwingWrapper;
import org.knowm.xchart.XYChart;

public class GUI extends JPanel {
    public static void main(String[] args) throws Exception {

        double phase = 0;
        double[][] initdata = getSineData(phase);

        final XYChart chart = QuickChart.getChart("Simple XChart Real-time Demo", "Radians", "Sine", "sine", initdata[0], initdata[1]);

        final SwingWrapper<XYChart> sw = new SwingWrapper<XYChart>(chart);
        sw.displayChart();

        while (true) {

            phase += 2 * Math.PI * 2 / 20.0;

            Thread.sleep(100);

            final double[][] data = getSineData(phase);

            chart.updateXYSeries("sine", data[0], data[1], null);
            sw.repaintChart();
        }

    }

    private static double[][] getSineData(double phase) {

        double[] xData = new double[100];
        double[] yData = new double[100];
        for (int i = 0; i < xData.length; i++) {
            double radians = phase + (2 * Math.PI / xData.length * i);
            xData[i] = radians;
            yData[i] = Math.sin(radians);
        }
        return new double[][] { xData, yData };
    }
}
