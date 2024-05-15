package pl.edu.pk.cosmo.rakieta.gui;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.DateAxis;
import org.jfree.chart.plot.XYPlot;
import org.jfree.data.time.DynamicTimeSeriesCollection;
import org.jfree.data.time.Second;

import javax.swing.*;
import java.awt.*;
import java.text.SimpleDateFormat;

public class GraphAnalyzer extends JPanel {
        private final DynamicTimeSeriesCollection dataset;
        private final JFreeChart chart;

        public GraphAnalyzer(final String title,int width, int height) {

            dataset = new DynamicTimeSeriesCollection(1, 1000, new Second());
            dataset.setTimeBase(new Second(0, 0, 0, 23, 1, 2024));
            dataset.addSeries(new float[1], 0, title);
            chart = ChartFactory.createTimeSeriesChart(
                    title, "Time", title, dataset, true, true, false);
            XYPlot plot = chart.getXYPlot();
            DateAxis axis = (DateAxis) plot.getDomainAxis();
            axis.setFixedAutoRange(10000);
            axis.setDateFormatOverride(new SimpleDateFormat("ss.SS"));
            final JPanel parent = this;
            ChartPanel chartPanel = new ChartPanel(chart){
                @Override
                public Dimension getPreferredSize() {
                    return new Dimension(width, height);
                }
            };
            add(chartPanel);
        }

        public void update(float value) {
            float[] newData = new float[1];
            newData[0] = value;
            dataset.advanceTime();
            dataset.appendData(newData);
        }
//        @Override
//        public Dimension getPreferredSize() {
//            return new Dimension(100, 100);
//        }


}
