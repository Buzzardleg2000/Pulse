/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;
using static pulse.cdm.bind.DataRequestData.Types;

namespace Pulse.CDM
{
  public class PBDataRequest
  {
    public static void Load(pulse.cdm.bind.DataRequestManagerData src, SEDataRequestManager dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.DataRequestManagerData src, SEDataRequestManager dst)
    {
      dst.SetResultsFilename(src.ResultsFilename);
      dst.SetSamplesPerSecond(src.SamplesPerSecond);
      for (int i = 0; i < src.DataRequest.Count; i++)
      {
        pulse.cdm.bind.DataRequestData dr = src.DataRequest[i];
        SEDataRequest dst_dr = SEDataRequest.New((eDataRequest_Category)dr.Category,
                                                 dr.ActionName,
                                                 dr.CompartmentName,
                                                 dr.SubstanceName,
                                                 dr.PropertyName,
                                                 new Unit(dr.Unit));
        if (dr.DecimalFormat != null)
        {
          dst_dr.GetDecimalFormat().precision = dr.DecimalFormat.Precision;
          dst_dr.GetDecimalFormat().type = (eDecimalFormatType)dr.DecimalFormat.Type;
        }
        dst.GetDataRequests().Add(dst_dr);
      }
    }
    public static bool SerializeFromString(string src, SEDataRequestManager dst, eSerializationFormat format)
    {
      try
      {
        pulse.cdm.bind.DataRequestManagerData data = JsonParser.Default.Parse<pulse.cdm.bind.DataRequestManagerData>(src);
        PBDataRequest.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.DataRequestManagerData Unload(SEDataRequestManager src)
    {
      pulse.cdm.bind.DataRequestManagerData dst = new pulse.cdm.bind.DataRequestManagerData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEDataRequestManager src, pulse.cdm.bind.DataRequestManagerData dst)
    {
      if (src.HasResultsFilename())
        dst.ResultsFilename = src.GetResultFilename();
      dst.SamplesPerSecond = src.GetSamplesPerSecond();
      foreach (var dr in src.GetDataRequests())
      {
        pulse.cdm.bind.DataRequestData dst_dr = new pulse.cdm.bind.DataRequestData();
        dst_dr.Category = (eCategory)dr.GetCategory();
        if (dr.HasActionName())
          dst_dr.ActionName = dr.GetActionName();
        if (dr.HasCompartmentName())
          dst_dr.CompartmentName = dr.GetCompartmentName();
        if (dr.HasSubstanceName())
          dst_dr.SubstanceName = dr.GetSubstanceName();
        if (dr.HasPropertyName())
          dst_dr.PropertyName = dr.GetPropertyName();
        if (dr.HasUnit())
          dst_dr.Unit = dr.GetUnit().ToString();
        if (dr.HasDecimalFormat())
        {
          dst_dr.DecimalFormat = new pulse.cdm.bind.DecimalFormatData();
          dst_dr.DecimalFormat.Type =
            (pulse.cdm.bind.DecimalFormatData.Types.eType)dr.GetDecimalFormat().type;
          dst_dr.DecimalFormat.Precision = dr.GetDecimalFormat().precision;
        }
        dst.DataRequest.Add(dst_dr);
      }
    }
    public static string SerializeToString(SEDataRequestManager src, eSerializationFormat format)
    {
      var pb = PBDataRequest.Unload(src);
      return pb.ToString();
    }
  }
}
