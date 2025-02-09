/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.utilities.csv.plots;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.io.File;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartUtils;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.LogarithmicAxis;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.StandardXYItemRenderer;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import com.kitware.pulse.utilities.DoubleUtils;
import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.csv.CSVContents;
import com.kitware.pulse.utilities.csv.plots.PlotDriver.PlotJob;

public class MultiPlotter implements Plotter
{
  protected Map<String,List<Double>> data = new HashMap<>();
  protected Map<String,List<Double>> expData = new HashMap<>();
  
  public static void main(String[] args)
  {
    PlotDriver.main(args);
  }

  @Override
  public void plot(PlotJob job)
  {
    //fill PlotJob with needed data if it doesn't exist
    if(job.dataPath == null || job.dataPath.isEmpty())
    {job.dataPath = job.verificationDirectory+"/";}
    if(job.dataFile == null || job.dataFile.isEmpty())
    {job.dataFile = job.name + "Results.csv";}
    
    //Get data contents for all headers when all data is in one file
    if ((data.isEmpty() || data == null) && job.experimentalData == null)
    {
      try
      {
        CSVContents csv = new CSVContents(job.dataPath + job.dataFile);
        csv.abreviateContents = job.resultsSkipNum;
        for (int i = 0; i < job.headers.size(); i++)
        {
          if(data.containsKey(job.headers.get(i)))
            continue;
          List<Double> headerData = new ArrayList<Double>();
          csv.readHeader(csv.unitUnderscoreToSpace(job.headers.get(i)), headerData);
          if(job.xPercentOfBaseline)
          {
            double baseline = headerData.get(0);
            boolean invertX = job.headers.get(i).contentEquals(job.X1header);
            for(int j=0; j < headerData.size(); j++)
            {
              if(invertX)// And convert to %
                headerData.set(j, (1-(headerData.get(j)/baseline))*100);
              else
                headerData.set(j, headerData.get(j)/baseline);
            }
          }
          data.put(job.headers.get(i), headerData);
        }
        
      } catch (IOException e)
      {
        Log.error("Could not analyze file " + job.dataPath + job.dataFile);
        return;
      }
    }
    //Get data contents when we're also using experimental files
    else if ((data.isEmpty() || data == null) && job.experimentalData != null)
    {
      CSVContents dataCsv = null;
      CSVContents expCsv = null;
      
      try
      {
        dataCsv = new CSVContents(job.dataPath + job.dataFile);
        dataCsv.abreviateContents = job.resultsSkipNum;
      } catch (IOException e)
      {
        Log.error("Could not analyze file " + job.dataPath + job.dataFile);
      }
      try
      {
        expCsv = new CSVContents(job.experimentalData);
        expCsv.abreviateContents = 0;  //we'll use all of the experimental data
      } catch (IOException e)
      {
        Log.error("Could not analyze file " + job.experimentalData);
      }
      try
      {
        //Get data from X1 and Y1, which should always be from our normal data file
        for (int i = 0; i < job.Y1headers.size(); i++)
        {
          List<Double> headerData = new ArrayList<>();
          dataCsv.readHeader(dataCsv.unitUnderscoreToSpace(job.Y1headers.get(i)), headerData);
          if(job.yPercentOfBaseline)
          {
            double baseline = headerData.get(0);
            for(int j=0; j < headerData.size(); j++)
              headerData.set(j, (headerData.get(j)/baseline)*100);
          }
          data.put(job.Y1headers.get(i), headerData);
        }
        List<Double> headerData = new ArrayList<>();
        dataCsv.readHeader(dataCsv.unitUnderscoreToSpace(job.X1header), headerData);
        if(job.xPercentOfBaseline)
        {
          double baseline = headerData.get(0);
          for(int j=0; j < headerData.size(); j++)
            headerData.set(j, (1-(headerData.get(j)/baseline))*100);
        }
        data.put(job.X1header, headerData);  
        if(job.Y2headers.size()>0 && !job.X2header.isEmpty())
        {
          for (int i = 0; i < job.Y2headers.size(); i++)
          {
            List<Double> headerData2 = new ArrayList<>();
            expCsv.readHeader(expCsv.unitUnderscoreToSpace(job.Y2headers.get(i)), headerData2);
            expData.put(job.Y2headers.get(i), headerData2);
          }
          List<Double> headerData2 = new ArrayList<>();
          expCsv.readHeader(expCsv.unitUnderscoreToSpace(job.X2header), headerData2);
          expData.put(job.X2header, headerData2);
        }
      } catch (Exception e)
      {
        Log.error("A problem was encountered reading headers from files.");
      }
    }
    
    //Catch some errors
    if(job.Y2headers.size() > 0 && job.X2header == null)
    {
      Log.error("No X2 header specified for job "+job.name+". Each Y axis must have a corresponding X axis.");
      return;
    }
    
    //Make a dataSeries for desired headers and add to collection(s)
    CSVPlotTool plotTool = new CSVPlotTool();  //to leverage existing functions
    String title = job.name + "_";
    XYSeriesCollection dataSet1 = new XYSeriesCollection();
    XYSeriesCollection dataSet2 = new XYSeriesCollection();
    double maxY1 = 0;
    double minY1 = Double.MAX_VALUE;
    double maxY2 = 0;
    double minY2 = Double.MAX_VALUE;
    for(int i = 0; i < job.Y1headers.size(); i++)
    {
      XYSeries dataSeries;
      String yLabel;
      if(job.Y1headers.size()==1 && job.Y1Label != null && !job.Y1Label.isEmpty())
        yLabel = job.Y1Label;
      else
        yLabel = job.Y1headers.get(i);
      if(job.experimentalData != null)
        dataSeries = plotTool.createXYSeries(yLabel,data.get(job.X1header),data.get(job.Y1headers.get(i)));
      else
        dataSeries = plotTool.createXYSeries(yLabel,data.get(job.X1header),data.get(job.Y1headers.get(i)));
      dataSet1.addSeries(dataSeries);
      title = title + job.Y1headers.get(i) + "_";
      maxY1 = maxY1 < dataSeries.getMaxY() ? dataSeries.getMaxY() : maxY1;
      minY1 = minY1 > dataSeries.getMinY() ? dataSeries.getMinY() : minY1;
    }
    for(int i = 0; i < job.Y2headers.size(); i++)
    {
      XYSeries dataSeries;
      String yLabel;
      if(job.Y2headers.size()==1 && job.Y2Label != null && !job.Y2Label.isEmpty())
        yLabel = job.Y2Label;
      else
        yLabel = job.Y2headers.get(i);
      if(job.experimentalData != null)
        dataSeries = plotTool.createXYSeries(yLabel,expData.get(job.X2header),expData.get(job.Y2headers.get(i)));
      else
        dataSeries = plotTool.createXYSeries(yLabel,data.get(job.X2header),data.get(job.Y2headers.get(i)));
      dataSet2.addSeries(dataSeries);
      title = title + job.Y2headers.get(i) + "_";
      maxY2 = maxY2 < dataSeries.getMaxY() ? dataSeries.getMaxY() : maxY2;
      minY2 = minY2 > dataSeries.getMinY() ? dataSeries.getMinY() : minY2;
    }
    title = title + "vs_" + job.X1header;
    if(job.X2header != null && !job.X1header.equalsIgnoreCase(job.X2header))
      title = title + "_" + job.X2header;
    
    //Override the constructed title if desired
    if(job.titleOverride != null && !job.titleOverride.isEmpty() && !job.titleOverride.equalsIgnoreCase("None"))
      title = job.titleOverride;
    
    //set labels
    String XAxisLabel = job.X1header;
    String YAxisLabel = job.Y1headers.get(0);
    
    JFreeChart chart = ChartFactory.createXYLineChart(
        job.titleOverride!=null&&job.titleOverride.equalsIgnoreCase("None")?"":title,// chart title
        XAxisLabel,         // x axis label
        YAxisLabel,         // y axis label
        dataSet1,                   // data
        PlotOrientation.VERTICAL,  // orientation
        true,                      // include legend
        true,                      // tooltips
        false                      // urls
        );
    
    Log.info("Creating Graph "+title);
    XYPlot plot = (XYPlot) chart.getPlot();
    
    if (!job.logAxis)
    {
      // Determine Y1 range
      double resMax0 = maxY1;
      double resMin0 = minY1;
      if (Double.isNaN(resMax0) || Double.isNaN(resMin0))
        plot.getDomainAxis(0).setLabel("Range is NaN");
      if (DoubleUtils.isZero(resMin0))
        resMin0 = -0.001;
      if (DoubleUtils.isZero(resMax0))
        resMax0 = 0.001;
      double rangeLength = resMax0 - resMin0;
      ValueAxis yAxis = plot.getRangeAxis(0);
      if(rangeLength != 0)
        yAxis.setRange(resMin0 - 0.15 * rangeLength, resMax0 + 0.15 * rangeLength);//15% buffer so we can see top and bottom clearly  
      
      //Override the bounds if desired
      try
      {
        if(job.Y1LowerBound != null)
          yAxis.setLowerBound(job.Y1LowerBound);
        if(job.Y1UpperBound != null)
          yAxis.setUpperBound(job.Y1UpperBound);
      } catch (Exception e)
      {
        Log.error("Couldn't set Y bounds. You probably tried to set a bound on an axis that doesn't exist.");
      }
      
      plot.setRangeAxis(0,yAxis);
      
      //Add the second Y axis to the right side
      if(!job.Y2headers.isEmpty())
      {
        ValueAxis rightYAxis = new NumberAxis();
        // Determine Y2 range
        double resMax1 = maxY2;
        double resMin1 = minY2;
        if (Double.isNaN(resMax1) || Double.isNaN(resMin1))
          plot.getDomainAxis(1).setLabel("Range is NaN");
        if (DoubleUtils.isZero(resMin1))
          resMin1 = -0.001;
        if (DoubleUtils.isZero(resMax1))
          resMax1 = 0.001;
        rangeLength = resMax1 - resMin1;
        if(rangeLength != 0)
          rightYAxis.setRange(resMin1 - 0.15 * rangeLength, resMax1 + 0.15* rangeLength);
        rightYAxis.setLabel(job.Y2headers.get(0));
        
        //Override the bounds if desired
        try
        {
          if(job.Y2LowerBound != null)
            rightYAxis.setLowerBound(job.Y2LowerBound);
          if(job.Y2UpperBound != null)
            rightYAxis.setUpperBound(job.Y2UpperBound);
        } catch (Exception e)
        {
          Log.error("Couldn't set Y bounds. You probably tried to set a bound on an axis that doesn't exist.");
        }
        
        plot.setRangeAxis(1, rightYAxis);
      }
    }
    else
    {
      double resMin = minY1 < minY2 ? minY1 : minY2;
      if(resMin <= 0.0)
        resMin = .00001;
      LogarithmicAxis yAxis = new LogarithmicAxis("Log("+YAxisLabel+")");
      yAxis.setLowerBound(resMin);
      
      //Override the bounds if desired
      try
      {
        if(job.Y1LowerBound != null)
          yAxis.setLowerBound(job.Y1LowerBound);
        if(job.Y1UpperBound != null)
          yAxis.setUpperBound(job.Y1UpperBound);
      } catch (Exception e)
      {
        Log.error("Couldn't set Y bounds. You probably tried to set a bound on an axis that doesn't exist.");
      }
      
      plot.setRangeAxis(0, yAxis);
      
      if (!job.Y2headers.isEmpty())
      {
        LogarithmicAxis rightYAxis = new LogarithmicAxis("Log("
            + job.Y2headers.get(0)+")");
        rightYAxis.setLowerBound(resMin);
        
        //Override the bounds if desired
        try
        {
          if(job.Y2LowerBound != null)
            rightYAxis.setLowerBound(job.Y2LowerBound);
          if(job.Y2UpperBound != null)
            rightYAxis.setUpperBound(job.Y2UpperBound);
        } catch (Exception e)
        {
          Log.error("Couldn't set Y bounds. You probably tried to set a bound on an axis that doesn't exist.");
        }
        
        plot.setRangeAxis(1, rightYAxis);
      }
    }
    
    //Override X bounds if desired
    try
    {
      if(job.X1LowerBound != null)
        plot.getDomainAxis(0).setLowerBound(job.X1LowerBound);
      if(job.X1UpperBound != null)
        plot.getDomainAxis(0).setUpperBound(job.X1UpperBound);
      if(job.X2LowerBound != null)
        plot.getDomainAxis(1).setLowerBound(job.X2LowerBound);
      if(job.X2UpperBound != null)
        plot.getDomainAxis(1).setUpperBound(job.X2UpperBound);
    }
    catch(Exception e)
    {
      Log.error("Couldn't set X bounds. You probably tried to set a bound on an axis that doesn't exist.");
    }
    
    //Add the second dataset if necessary
    if(!job.Y2headers.isEmpty())
    {
      plot.setDataset(1, dataSet2);
      plot.mapDatasetToRangeAxis(1, 1);
    }
    
    //Override labels if desired
    if(job.X1Label != null && !plot.getDomainAxis(0).getLabel().contains("NaN"))
      plot.getDomainAxis(0).setLabel(job.X1Label.equalsIgnoreCase("None")?"":job.X1Label);
    if(job.X2Label != null && plot.getDomainAxis(1) != null)
      plot.getDomainAxis(1).setLabel(job.X2Label.equalsIgnoreCase("None")?"":job.X2Label);
    if(job.Y1Label != null)
      plot.getRangeAxis(0).setLabel(job.Y1Label.equalsIgnoreCase("None")?"":job.Y1Label);
    if(job.Y2Label != null && plot.getRangeAxis(1) != null)
      plot.getRangeAxis(1).setLabel(job.Y2Label.equalsIgnoreCase("None")?"":job.Y2Label);
    
    //Format lines and colors
    plot.setDomainGridlinesVisible(job.showGridLines);
    plot.setRangeGridlinesVisible(job.showGridLines);
    formatMultiPlot(job, chart, dataSet1, dataSet2);
    
    //Handle legends
    if(job.removeAllLegends)
      chart.removeLegend();
    
    //Make the file
    try
    {
      FileUtils.createDirectory(job.outputDir);
      String filename = job.outputFilename==null ? job.outputDir+"/"+plotTool.MakeFileName(title)+".jpg" : job.outputDir+"/"+job.outputFilename;
      if(!filename.endsWith(".jpg"))
        filename = filename + ".jpg";
      File JPGFile = new File(filename);
      if(job.imageHeight != null && job.imageWidth != null)
        ChartUtils.saveChartAsJPEG(JPGFile, chart, job.imageWidth, job.imageHeight);
      else
        ChartUtils.saveChartAsJPEG(JPGFile, chart, 1600, 800);
    }
    catch (IOException e)
    {
      Log.error(e.getMessage());
    }

  }
  
  protected void formatMultiPlot(PlotJob job, JFreeChart chart, XYSeriesCollection dataSet1, XYSeriesCollection dataSet2)
  {
    Color[] blueColors = {Color.blue, Color.cyan, new Color(0,160,255), new Color(0,100,255), new Color(0,160,255), new Color(14,0,145), new Color(70,105,150)};
    Color[] redColors = {Color.red, Color.magenta, new Color(255,0,100), new Color(255,0,160), Color.pink, new Color(145,0,0), new Color(132,58,58)};
    Color[] variedColors = {Color.red, Color.blue, Color.green, Color.orange, Color.magenta, Color.cyan, Color.gray, new Color(255,165,0), new Color(42,183,136), new Color(87,158,186)};
    XYPlot plot = (XYPlot) chart.getPlot();
    XYLineAndShapeRenderer renderer1 = (XYLineAndShapeRenderer) plot.getRenderer();
    BasicStroke wideLine = new BasicStroke( 2.0f ); 
    
    //For Scientific notation
    NumberFormat formatter = new DecimalFormat("0.###E0");
    
    for(int i = 0; i < plot.getDomainAxisCount(); i++)
    {
      plot.getDomainAxis(i).setLabelFont(new Font("SansSerif", Font.PLAIN, job.fontSize));
      plot.getDomainAxis(i).setTickLabelFont(new Font("SansSerif", Font.PLAIN, 15));
      plot.getDomainAxis(i).setLabelPaint(job.bgColor==Color.red?Color.white:Color.black);
      plot.getDomainAxis(i).setTickLabelPaint(job.bgColor==Color.red?Color.white:Color.black);
    }
    for(int i = 0; i < plot.getRangeAxisCount(); i++)
    {
      plot.getRangeAxis(i).setLabelFont(new Font("SansSerif", Font.PLAIN, job.fontSize));
      plot.getRangeAxis(i).setTickLabelFont(new Font("SansSerif", Font.PLAIN, 15));
      plot.getRangeAxis(i).setLabelPaint(job.bgColor==Color.red?Color.white:Color.black);
      plot.getRangeAxis(i).setTickLabelPaint(job.bgColor==Color.red?Color.white:Color.black);
      NumberAxis rangeAxis = (NumberAxis)plot.getRangeAxis(i);
      //rangeAxis.setNumberFormatOverride(formatter);
    }

    //White background outside of plottable area
    chart.setBackgroundPaint(job.bgColor);

    plot.setBackgroundPaint(Color.white);
    plot.setDomainGridlinePaint(Color.black);
    plot.setRangeGridlinePaint(Color.black);

    plot.setDomainCrosshairVisible(true);
    plot.setRangeCrosshairVisible(true);

    renderer1.setDefaultLegendTextFont(new Font("SansSerif", Font.PLAIN, job.legendFontSize));
    chart.getTitle().setFont(new Font("SansSerif", Font.PLAIN, job.fontSize));
    chart.getTitle().setPaint(job.bgColor==Color.red?Color.white:Color.black);
    
    //If there is only one Y axis, we just need to color the data series differently
    if(job.Y2headers == null || job.Y2headers.isEmpty())
    {
      for(int i = 0, cIndex = 0; i < dataSet1.getSeriesCount(); i++, cIndex++)
      {
        renderer1.setSeriesStroke(i, wideLine); 
        renderer1.setDefaultShapesVisible(false);//setBaseShapesVisible(false);
        if(cIndex > 9) cIndex = 0;
        renderer1.setSeriesFillPaint(i, variedColors[cIndex]); 
        renderer1.setSeriesPaint(i, variedColors[cIndex]);
      }
    }
    //If there are 2 Y axes, we should color the axes to correspond with the data so it isn't (as) confusing
    else
    {
      StandardXYItemRenderer renderer2 = new StandardXYItemRenderer();
      renderer2.setDefaultLegendTextFont(new Font("SansSerif", Font.PLAIN, job.legendFontSize));
      plot.setRenderer(1, renderer2);
      
      for(int i = 0, cIndex = 0; i < dataSet1.getSeriesCount(); i++, cIndex++)
      {
        renderer1.setSeriesStroke(i, wideLine); 
        renderer1.setDefaultShapesVisible(false);//setBaseShapesVisible(false);
        if(cIndex > 6) cIndex = 0;
        renderer1.setSeriesFillPaint(i, redColors[cIndex]); 
        renderer1.setSeriesPaint(i, redColors[cIndex]);
      }
      for(int i = 0, cIndex = 0; i < dataSet2.getSeriesCount(); i++, cIndex++)
      {
        renderer2.setSeriesStroke(i, wideLine); 
        renderer2.setBaseShapesVisible(false);
        if(cIndex > 6) cIndex = 0;
        renderer2.setSeriesFillPaint(i, blueColors[cIndex]); 
        renderer2.setSeriesPaint(i, blueColors[cIndex]);
      }
      plot.getRangeAxis(0).setLabelPaint(redColors[0]);
      plot.getRangeAxis(0).setTickLabelPaint(redColors[0]);
      plot.getRangeAxis(1).setLabelPaint(blueColors[0]);
      plot.getRangeAxis(1).setTickLabelPaint(blueColors[0]);
    }
  }

  

}
