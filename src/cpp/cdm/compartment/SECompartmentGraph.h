/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#define COMPARTMENT_GRAPH_TEMPLATE typename CompartmentType, typename CompartmentLinkType
#define COMPARTMENT_GRAPH_TYPES CompartmentType, CompartmentLinkType

template<COMPARTMENT_GRAPH_TEMPLATE>
class SECompartmentGraph : public Loggable
{
  friend class PBCompartment;//friend the serialization class
protected:
  SECompartmentGraph(const std::string& name, Logger* logger);
public:
  virtual ~SECompartmentGraph();

  virtual void Clear();

  virtual std::string GetName() const;

  virtual void AddCompartment(CompartmentType& cmpt);
  virtual void ForceAddCompartment(CompartmentType& cmpt);// No check for cmpt with the same name
  virtual CompartmentType* GetCompartment(const std::string& name);
  virtual const CompartmentType* GetCompartment(const std::string& name) const;  
  virtual const std::vector<CompartmentType*>& GetCompartments() const;
  virtual void RemoveCompartment(const CompartmentType& cmpt);
  virtual void RemoveCompartment(const std::string& name);

  virtual void AddLink(CompartmentLinkType& link);
  virtual void ForceAddLink(CompartmentLinkType& link);// No check for link with the same name
  virtual CompartmentLinkType* GetLink(const std::string& name);
  virtual const CompartmentLinkType* GetLink(const std::string& name) const;
  virtual const std::vector<CompartmentLinkType*>& GetLinks() const;
  virtual void RemoveLink(const CompartmentLinkType& link);
  virtual void RemoveLink(const std::string& name);  

protected:
  std::string                       m_Name;
  std::vector<CompartmentType*>     m_Compartments;
  std::vector<CompartmentLinkType*> m_CompartmentLinks;
};

#include "cdm/compartment/SECompartmentGraph.hxx"
