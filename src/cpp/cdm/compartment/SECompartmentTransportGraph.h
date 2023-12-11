/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/compartment/SECompartmentGraph.h"

#define COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE typename GraphType, typename GraphVertexType, typename GraphEdgeType, typename CompartmentType, typename CompartmentLinkType
#define COMPARTMENT_TRANSPORT_GRAPH_TYPES GraphType, GraphVertexType, GraphEdgeType, CompartmentType, CompartmentLinkType

template<COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
class SECompartmentTransportGraph : public GraphType, public SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>
{
public:
  SECompartmentTransportGraph(const std::string& name, Logger* logger);
  virtual ~SECompartmentTransportGraph();

  void Clear() override;

  std::string GetName() const override{ return this->m_Name; }

  virtual void StateChange();

protected:
  void BalanceByIntensive() override = 0;
  int GetVertexIndex(const GraphVertexType& v) const override;
  const std::vector<GraphVertexType*>& GetVerticies() const override;
  const std::vector<GraphEdgeType*>* GetSourceEdges(const GraphVertexType& v) const override;
  const std::vector<GraphEdgeType*>* GetTargetEdges(const GraphVertexType& v) const override;

  std::vector<GraphVertexType*>                                  m_Verticies;
  std::map<const GraphVertexType*, int>                          m_VertexIndicies;
  std::map<const GraphVertexType*, std::vector<GraphEdgeType*>*> m_SourceEdgeMap;
  std::map<const GraphVertexType*, std::vector<GraphEdgeType*>*> m_TargetEdgeMap;
};

#include "cdm/compartment/SECompartmentTransportGraph.hxx"
