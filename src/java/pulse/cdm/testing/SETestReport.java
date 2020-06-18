/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.testing;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import pulse.cdm.bind.TestReport.TestReportData;
import pulse.cdm.bind.TestReport.TestSuiteData;
import pulse.cdm.properties.CommonUnits.TimeUnit;
import pulse.utilities.FileUtils;
import pulse.utilities.Log;

/**
 * @author abray, boday
 *
 */
public class SETestReport
{  
  protected String            name;
  protected String            fileName;
  protected String            reportDir;
  protected List<SETestSuite> testSuites;
  protected List<String>      knownFailingSuites;

  private class Data
  {
    public String          name;
    public int             runs;
    public int             errors;
    public double          duration_s;
    public List<String>    failures = new ArrayList<>();
    public List<String>    requirements = new ArrayList<>();
    public StringBuilder   html=new StringBuilder();
  }

  private class DataComparator implements Comparator<Data>
  {
    @Override
    public int compare(Data o1, Data o2) 
    {
      return o1.name.compareTo(o2.name);
    }
  }

  public SETestReport()
  {
    this.reportDir="./";
    setFileName("TestReport.json");
    testSuites = new ArrayList<>();
    knownFailingSuites = new ArrayList<>();
  }
  
  public void reset()
  {
  	name="";
  	fileName="";
  	reportDir="";
  	testSuites.clear();
  	knownFailingSuites.clear();
  }

  public void setFullReportPath(String path)
  {
    path = path.replaceAll("[\\\\]", "/");
    String dir = path.substring(0,path.lastIndexOf("/")+1);
    setReportDirectory(dir);
    String file = path.substring(path.lastIndexOf("/")+1);
    setFileName(file);
  }

  public void setReportDirectory(String dir)
  {
    try
    {
      FileUtils.createDirectory(dir);
      this.reportDir=dir+"/";
    }
    catch(Exception ex)
    {
      Log.error(ex);
      this.reportDir="./";
    }
  }

  public String getName()          { return this.name;     }
  public void setName(String name) { this.name = name;     }
  public String getFileName()      { return this.fileName; }
  public void setFileName(String name, String extension)
  {
    this.name = name;
    this.fileName=name+extension;
  }
  public void setFileName(String fileName)
  {
    if(fileName.indexOf(".")>=0)
    {
      this.fileName=fileName;
      this.name = fileName.substring(0,fileName.lastIndexOf("."));
    }
    else
    {
      this.name = fileName;
      this.fileName=fileName+".json";// make it an json file
    }
  }

  public void readFile(String fileName) throws InvalidProtocolBufferException
  {
  	TestReportData.Builder builder = TestReportData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    SETestReport.load(builder.build(), this);
  }
  public void writeFile(String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(SETestReport.unload(this)));
  }
  public static void load(TestReportData src, SETestReport dst)
  {
  	dst.reset();
  	for(TestSuiteData tsd : src.getTestSuiteList())
  	{
  		SETestSuite ts = dst.createTestSuite();
  		SETestSuite.load(tsd, ts);
  	}
  }
	public static TestReportData unload(SETestReport src)
  {
		TestReportData.Builder dst = TestReportData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SETestReport src, TestReportData.Builder dst)
  {
    for(SETestSuite ts : src.testSuites)
    	dst.addTestSuite(SETestSuite.unload(ts));
  }

  public int getErrors()
  {
    int errors=0;
    for(SETestSuite suite : this.testSuites)
      errors+=suite.getNumErrors();
    return errors;
  }

  public SETestSuite createTestSuite()
  {
  	SETestSuite ts = new SETestSuite();
  	this.testSuites.add(ts);
    return ts;
  }
  public void createErrorSuite(String name, String error)
  {
  	SETestSuite ts = new SETestSuite();
  	ts.setName(name);
  	ts.setPerformed(false);
  	SETestCase  tc = ts.createTestCase();
  	tc.setName(name);
  	tc.AddFailure(error);
    this.testSuites.add(ts);
  }

  public void addKnownSuiteFailure(String suiteName)
  {
    knownFailingSuites.add(suiteName);
  }

  public void addTestSuite(SETestSuite ts)
  {
    this.testSuites.add(ts);
  }

  public void write() throws InvalidProtocolBufferException
  {
    write(this.reportDir);
  }

  public void write(String toDirectory) throws InvalidProtocolBufferException
  {
    if(toDirectory==null||toDirectory.isEmpty())
      toDirectory="./";
    this.writeFile(toDirectory + this.fileName);
  }

  public String toHTML(String title)
  {
    return toHTML(title, null);
  }

  public String toHTML(String title, Map<String,List<String>> groups)
  {
    boolean sortResults = true;
    StringBuilder buffer = new StringBuilder();
    buffer.append("<html>");
    buffer.append("<head><title>"+title+"</title></head>");
    buffer.append("<body>");
    buffer.append("<h1>"+title+"</h1><br>");
    
    // Any Test Suites Not Run?
    buffer.append("<table border=\"1\">");
    buffer.append("<tr><th>Missing Reports</th></tr>");
    for(SETestSuite ts : this.testSuites)
    {
      if(!ts.getPerformed())
        buffer.append("<tr bgcolor=\"#FF0000\"><td>"+ts.getName()+"</td></tr>");
    }
    buffer.append("</table>");
    // Make a little room
    buffer.append("<br>");
    
    // Any suites known to fail?
    buffer.append("<table border=\"1\">");
    buffer.append("<tr><th>Known Failing Reports</th></tr>");
    for(String suite : this.knownFailingSuites)
    {
      buffer.append("<tr bgcolor=\"#FFFF00\"><td>"+suite+"</td></tr>");
    }
    buffer.append("</table>");

    if(groups == null)
    {
      groups = new HashMap<>();
      List<String> all = new ArrayList<>();
      for(SETestSuite ts : this.testSuites)
        all.add(ts.getName());
      groups.put(this.name, all);
    }
    
    for(String group : groups.keySet())
    {
      List<String> groupTests = groups.get(group);
      // Make a little room
      buffer.append("<br>");

      // Report Suite Summary Results
      buffer.append("<table border=\"1\">");
      buffer.append("<caption><b>"+group+"</b></caption>");
      buffer.append("<tr>");
      buffer.append("<th> Test Case </th>");      
      buffer.append("<th> # of Failures </th>");
      buffer.append("<th> # of Runs </th>");
      buffer.append("<th> Time </th>");
      buffer.append("<th> Failures </th>");
      buffer.append("<th> Requirements Tested </th>");
      buffer.append("</tr>");


      int runs=0;
      int totalRuns=0;
      int totalErrors=0;
      double totalDuration_s=0;
      List<SETestReport.Data> errorData = new ArrayList<>();
      List<SETestReport.Data> passedData = new ArrayList<>();

      SETestReport.Data data;
      for(SETestSuite ts : this.testSuites)
      { 
        if(!groupTests.contains(ts.getName()))
            continue;
        
        totalRuns+=ts.getTestCases().size();
        totalErrors+=ts.getNumErrors();
        totalDuration_s+=ts.getDuration(TimeUnit.s);


        if(ts.getPerformed())
        {
          runs++;
          data=new Data();
          data.name=ts.getName();
          data.runs=ts.getTestCases().size();
          data.errors=ts.getNumErrors();        
          data.duration_s=ts.getDuration(TimeUnit.s);
          for(SETestCase tc : ts.testCases)
          	data.failures.addAll(tc.failures);
          data.requirements.addAll(ts.getRequirements());
          if(ts.getNumErrors()>0)
          {
            errorData.add(data);
            data.html.append("<tr bgcolor=\"#FF0000\">");
          }
          else
          {
            passedData.add(data);
            data.html.append("<tr bgcolor=\"#00FF00\">");
          }
          data.html.append("<td align=\"left\">"+data.name+"</td>");   
          data.html.append("<td>"+data.errors+"</td>");
          data.html.append("<td>"+data.runs+"</td>");  
          data.html.append("<td>"+data.duration_s+"</td>");
          data.html.append("<td>");
          if(data.failures.size() > 0)
          {
            int i=0;
            while (i < data.failures.size()-1)
            {
              data.html.append(data.failures.get(i) + "<br>");
              i++;
              if(i>5)//Only write out the first few errors, could be a LOT of errors
                break;
            }
            data.html.append(data.failures.get(i));
          }
          data.html.append("</td>");
          data.html.append("<td>");
          if (data.requirements.size() > 0)
          {
            int i=0;
            while (i < data.requirements.size()-1)
            {
              data.html.append(data.requirements.get(i) + " , ");
              i++;
            }
            data.html.append(data.requirements.get(i));
          }
          data.html.append("</td>");
          data.html.append("</tr>");
        }
      }

      buffer.append("<tr>");
      buffer.append("<td align=\"left\">"+"Totals for "+runs+" test suites"+"</td>");   
      buffer.append("<td>"+totalErrors+"</td>");
      buffer.append("<td>"+totalRuns+"</td>");  
      buffer.append("<td>"+totalDuration_s+"</td>"); 
      buffer.append("</tr>");

      if(sortResults)
      {
        DataComparator comparator=new DataComparator();
        Collections.sort(errorData,comparator);
        Collections.sort(passedData,comparator);
      }
      for(Data d : errorData)
        buffer.append(d.html);
      for(Data d : passedData)
        buffer.append(d.html);

      buffer.append("</table>");

      // Make a little room
      buffer.append("<br>");
    }

    buffer.append("</body>");
    buffer.append("</html>");
    return buffer.toString();
  }
}
