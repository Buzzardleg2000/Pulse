/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once

#include "circuit/SECircuitManager.h"
#include "compartment/fluid/SEGasCompartmentGraph.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"
#include "compartment/thermal/SEThermalCompartment.h"
#include "compartment/thermal/SEThermalCompartmentLink.h"
#include "compartment/tissue/SETissueCompartment.h"
#include "bind/cdm/Compartment.pb.h"

class DLL_DECL SECompartmentManager : public Loggable
{
public:
  SECompartmentManager(SESubstanceManager& subMgr);
  virtual ~SECompartmentManager();

  virtual void Clear(); //clear memory

  bool LoadFile(const std::string& filename);
  void SaveFile(const std::string& filename);

  static void Load(const cdm::CompartmentManagerData& src, SECompartmentManager& dst, SECircuitManager* circuits = nullptr);
  static cdm::CompartmentManagerData* Unload(const SECompartmentManager& src);
protected:
  static void Serialize(const cdm::CompartmentManagerData& src, SECompartmentManager& dst, SECircuitManager* circuits = nullptr);
  static void Serialize(const SECompartmentManager& src, cdm::CompartmentManagerData& dst);

public:
  virtual void                                                StateChange();// Identify leaves and other bookkeeping
  virtual void                                                UpdateLinks();
  virtual void                                                UpdateLinks(SEGasCompartmentGraph& graph);
  virtual void                                                UpdateLinks(SELiquidCompartmentGraph& graph);

  virtual bool                                                HasCompartment(cdm::eCompartmentType type, const std::string& name) const;
  virtual SECompartment*                                      GetCompartment(cdm::eCompartmentType type, const std::string& name);
  virtual const SECompartment*                                GetCompartment(cdm::eCompartmentType type, const std::string& name) const;

  virtual SEGasCompartment&                                   CreateGasCompartment(const std::string& name);
  virtual void                                                DeleteGasCompartment(const std::string& name);
  virtual bool                                                HasGasCompartment(const std::string& name) const;
  virtual SEGasCompartment*                                   GetGasCompartment(const std::string& name);
  virtual const SEGasCompartment*                             GetGasCompartment(const std::string& name) const;
  virtual const std::vector<SEGasCompartment*>&               GetGasCompartments();
  virtual const std::vector<SEGasCompartment*>&               GetGasLeafCompartments();
  virtual SEGasCompartmentLink&                               CreateGasLink(SEGasCompartment& src, SEGasCompartment& tgt, const std::string& name);
  virtual void                                                DeleteGasLink(const std::string& name);
  virtual bool                                                HasGasLink(const std::string& name) const;
  virtual SEGasCompartmentLink*                               GetGasLink(const std::string& name);
  virtual const SEGasCompartmentLink*                         GetGasLink(const std::string& name) const;
  virtual const std::vector<SEGasCompartmentLink*>&           GetGasLinks();
  virtual SEGasCompartmentGraph&                              CreateGasGraph(const std::string& name);
  virtual void                                                DeleteGasGraph(const std::string& name);
  virtual bool                                                HasGasGraph(const std::string& name) const;
  virtual SEGasCompartmentGraph*                              GetGasGraph(const std::string& name);
  virtual const SEGasCompartmentGraph*                        GetGasGraph(const std::string& name) const;
  virtual const std::vector<SEGasCompartmentGraph*>&          GetGasGraphs();
  virtual void                                                AddGasCompartmentSubstance(SESubstance& sub);
  virtual const std::vector<SESubstance*>&                    GetGasCompartmentSubstances() const;
  
  virtual SELiquidCompartment&                                CreateLiquidCompartment(const std::string& name);
  virtual void                                                DeleteLiquidCompartment(const std::string& name);
  virtual bool                                                HasLiquidCompartment(const std::string& name) const;
  virtual SELiquidCompartment*                                GetLiquidCompartment(const std::string& name);
  virtual const SELiquidCompartment*                          GetLiquidCompartment(const std::string& name) const;
  virtual const std::vector<SELiquidCompartment*>&            GetLiquidCompartments();
  virtual const std::vector<SELiquidCompartment*>&            GetLiquidLeafCompartments();
  virtual SELiquidCompartmentLink&                            CreateLiquidLink(SELiquidCompartment& src, SELiquidCompartment& tgt, const std::string& name);
  virtual void                                                DeleteLiquidLink(const std::string& name);
  virtual bool                                                HasLiquidLink(const std::string& name) const;
  virtual SELiquidCompartmentLink*                            GetLiquidLink(const std::string& name);
  virtual const SELiquidCompartmentLink*                      GetLiquidLink(const std::string& name) const;
  virtual const std::vector<SELiquidCompartmentLink*>&        GetLiquidLinks();
  virtual SELiquidCompartmentGraph&                           CreateLiquidGraph(const std::string& name);
  virtual void                                                DeleteLiquidGraph(const std::string& name);
  virtual bool                                                HasLiquidGraph(const std::string& name) const;
  virtual SELiquidCompartmentGraph*                           GetLiquidGraph(const std::string& name);
  virtual const SELiquidCompartmentGraph*                     GetLiquidGraph(const std::string& name) const;
  virtual const std::vector<SELiquidCompartmentGraph*>&       GetLiquidGraphs();
  virtual void                                                AddLiquidCompartmentSubstance(SESubstance& sub);
  virtual const std::vector<SESubstance*>&                    GetLiquidCompartmentSubstances() const;
  
  virtual SEThermalCompartment&                               CreateThermalCompartment(const std::string& name);
  virtual void                                                DeleteThermalCompartment(const std::string& name);
  virtual bool                                                HasThermalCompartment(const std::string& name) const;
  virtual SEThermalCompartment*                               GetThermalCompartment(const std::string& name);
  virtual const SEThermalCompartment*                         GetThermalCompartment(const std::string& name) const;
  virtual const std::vector<SEThermalCompartment*>&           GetThermalCompartments();
  virtual const std::vector<SEThermalCompartment*>&           GetThermalLeafCompartments();
  virtual SEThermalCompartmentLink&                           CreateThermalLink(SEThermalCompartment& src, SEThermalCompartment& tgt, const std::string& name);
  virtual void                                                DeleteThermalLink(const std::string& name);
  virtual bool                                                HasThermalLink(const std::string& name) const;
  virtual SEThermalCompartmentLink*                           GetThermalLink(const std::string& name);
  virtual const SEThermalCompartmentLink*                     GetThermalLink(const std::string& name) const;
  virtual const std::vector<SEThermalCompartmentLink*>&       GetThermalLinks();
  
  virtual bool                                                HasTissueCompartment(const std::string& name) const;
  virtual void                                                DeleteTissueCompartment(const std::string& name);
  virtual SETissueCompartment&                                CreateTissueCompartment(const std::string& name);
  virtual SETissueCompartment*                                GetTissueCompartment(const std::string& name);
  virtual const SETissueCompartment*                          GetTissueCompartment(const std::string& name) const;
  virtual const std::vector<SETissueCompartment*>&            GetTissueCompartments();
  virtual const std::vector<SETissueCompartment*>&            GetTissueLeafCompartments();
 
protected:
  std::vector<SEGasCompartment*>                              m_GasCompartments;
  std::vector<SEGasCompartment*>                              m_GasLeafCompartments;
  std::map<std::string, SEGasCompartment*>                    m_GasName2Compartments;
  std::vector<SEGasCompartmentLink*>                          m_GasLinks;
  std::map<std::string, SEGasCompartmentLink*>                m_GasName2Links;
  std::vector<SEGasCompartmentGraph*>                         m_GasGraphs;
  std::map<std::string, SEGasCompartmentGraph*>               m_GasName2Graphs;
  std::vector<SESubstance*>                                   m_GasSubstances;
  
  std::vector<SELiquidCompartment*>                           m_LiquidCompartments;
  std::vector<SELiquidCompartment*>                           m_LiquidLeafCompartments;
  std::map<std::string, SELiquidCompartment*>                 m_LiquidName2Compartments;
  std::vector<SELiquidCompartmentLink*>                       m_LiquidLinks;
  std::map<std::string, SELiquidCompartmentLink*>             m_LiquidName2Links;
  std::vector<SELiquidCompartmentGraph*>                      m_LiquidGraphs;
  std::map<std::string, SELiquidCompartmentGraph*>            m_LiquidName2Graphs;
  std::vector<SESubstance*>                                   m_LiquidSubstances;
  
  std::vector<SEThermalCompartment*>                          m_ThermalCompartments;
  std::vector<SEThermalCompartment*>                          m_ThermalLeafCompartments;
  std::map<std::string, SEThermalCompartment*>                m_ThermalName2Compartments;
  std::vector<SEThermalCompartmentLink*>                      m_ThermalLinks;
  std::map<std::string, SEThermalCompartmentLink*>            m_ThermalName2Links;

  std::vector<SETissueCompartment*>                           m_TissueCompartments;
  std::vector<SETissueCompartment*>                           m_TissueLeafCompartments;
  std::map<std::string, SETissueCompartment*>                 m_TissueName2Compartments;

  template<typename CompartmentType, typename LinkType>
  void UpdateLinks(const std::vector<CompartmentType*>& compartments, const std::vector<LinkType*>& links) const;

  template<typename CompartmentType, typename LinkType>
  void GetChildLinks(CompartmentType* pnt, CompartmentType* child) const;

  template<typename CompartmentType>
  void FindLeaves(CompartmentType* cmpt, std::vector<CompartmentType*>& leaves) const;

  template<typename CompartmentType>
  CompartmentType& CreateCompartment(const std::string& name, std::map<std::string, CompartmentType*>& name2cmpt, std::vector<SESubstance*>* substances);

  template<typename CompartmentType>
  bool HasCompartment(const std::string& name, const std::map<std::string, CompartmentType*>& name2cmpt) const;

  template<typename CompartmentType>
  CompartmentType* GetCompartment(const std::string& name, std::map<std::string, CompartmentType*>& name2cmpt) const;

  template<typename CompartmentType>
  const CompartmentType* GetCompartment(const std::string& name, const std::map<std::string, CompartmentType*>& name2cmpt) const;

  template<typename LinkType, typename CompartmentType>
  LinkType& CreateLink(CompartmentType& src, CompartmentType& tgt, const std::string& name, std::map<std::string, LinkType*>& name2link) const;

  template<typename LinkType>
  bool HasLink(const std::string& name, const std::map<std::string, LinkType*> &name2link) const;

  template<typename LinkType>
  LinkType* GetLink(const std::string& name, std::map<std::string, LinkType*>& name2link) const;

  template<typename LinkType>
  const LinkType* GetLink(const std::string& name, const std::map<std::string, LinkType*>& name2link) const;

  template<typename CompartmentType>
  void SetSubstances(CompartmentType& cmpt, std::vector<SESubstance*>& substances) const;
  
  template<typename CompartmentType>
  void AddSubstance(SESubstance& s, CompartmentType& cmpt) const;

  // By default, every compartment gets every substance. Override these if you want to discritize better who gets what
  virtual bool AllowGasSubstance(SESubstance& s, SEGasCompartment& cmpt) const { return true; }
  virtual bool AllowLiquidSubstance(SESubstance& s, SELiquidCompartment& cmpt) const { return true; }


  SESubstance* m_O2;
  SESubstance* m_CO2;
  SESubstance* m_CO;
  SESubstance* m_Hb;
  SESubstance* m_HbO2;
  SESubstance* m_HbCO2;
  SESubstance* m_HbO2CO2;
  SESubstance* m_HbCO;
  SESubstanceManager& m_subMgr;
};