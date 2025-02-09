/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#ifdef MSVC
  #pragma warning(disable : 4503)
#endif

#define SUBSTANCE_TRANSPORTER_TEMPLATE typename GraphType, typename FluxUnit, typename QuantityUnit, typename ExtensiveUnit, typename IntensiveUnit

#define TRANSPORT_AMOUNT_TYPES ExtensiveScalar, IntensiveScalar
template<typename ExtensiveScalar, typename IntensiveScalar>
class CDM_DECL SESubstanceTransportAmount
{
  template<SUBSTANCE_TRANSPORTER_TEMPLATE> friend class SESubstanceTransporter;
public:
  virtual ~SESubstanceTransportAmount() {}

  virtual void Clear() = 0;

  virtual bool HasExtensive() const = 0;
  virtual ExtensiveScalar& GetExtensive() = 0;

  virtual bool HasIntensive() const = 0;
  virtual IntensiveScalar& GetIntensive() = 0;
};
using SEGasTransportSubstance = SESubstanceTransportAmount<SEScalarVolume, SEScalar0To1>;
using SELiquidTransportSubstance = SESubstanceTransportAmount<SEScalarMass, SEScalarMassPerVolume>;

#define TRANSPORT_VERTEX_TYPES QuantityScalar, ExtensiveScalar, IntensiveScalar
template <typename QuantityScalar, typename ExtensiveScalar, typename IntensiveScalar>
class CDM_DECL SESubstanceTransportVertex
{
  template<SUBSTANCE_TRANSPORTER_TEMPLATE> friend class SESubstanceTransporter;
public:
  virtual ~SESubstanceTransportVertex() {}

  virtual std::string GetName() const = 0;

protected:
  virtual bool HasQuantity() const = 0;
  virtual QuantityScalar& GetQuantity() = 0;

  virtual std::vector<SESubstanceTransportAmount<TRANSPORT_AMOUNT_TYPES>*>& GetTransportSubstances() = 0;
};
using SEGasTransportVertex = SESubstanceTransportVertex<SEScalarVolume, SEScalarVolume, SEScalar0To1>;
using SELiquidTransportVertex = SESubstanceTransportVertex<SEScalarVolume, SEScalarMass, SEScalarMassPerVolume>;

#define TRANSPORT_EDGE_TYPES FluxScalar, QuantityScalar, ExtensiveScalar, IntensiveScalar
template <typename FluxScalar, typename QuantityScalar, typename ExtensiveScalar, typename IntensiveScalar>
class CDM_DECL SESubstanceTransportEdge
{
  template<SUBSTANCE_TRANSPORTER_TEMPLATE> friend class SESubstanceTransporter;
public:
  virtual ~SESubstanceTransportEdge() {}

  virtual std::string GetName() const = 0;

protected:
  virtual bool HasFlux() const = 0;
  virtual FluxScalar& GetFlux() = 0;

  virtual SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>& GetSourceVertex() = 0;
  virtual SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>& GetTargetVertex() = 0;
};
using SEGasTransportEdge = SESubstanceTransportEdge<SEScalarVolumePerTime, SEScalarVolume, SEScalarVolume, SEScalar0To1>;
using SELiquidTransportEdge = SESubstanceTransportEdge<SEScalarVolumePerTime, SEScalarVolume, SEScalarMass, SEScalarMassPerVolume>;

template <typename FluxScalar, typename QuantityScalar, typename ExtensiveScalar, typename IntensiveScalar>
class CDM_DECL SESubstanceTransportGraph
{
  template<SUBSTANCE_TRANSPORTER_TEMPLATE> friend class SESubstanceTransporter;
public:
  virtual ~SESubstanceTransportGraph() {}

protected:
  virtual void BalanceByIntensive() = 0;// Transporter calculates the new concentration
  virtual int GetVertexIndex(const SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>& v) const = 0;
  virtual const std::vector<SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>*>& GetVerticies() const = 0;
  virtual const std::vector<SESubstanceTransportEdge<TRANSPORT_EDGE_TYPES>*>* GetSourceEdges(const SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>& v) const = 0;
  virtual const std::vector<SESubstanceTransportEdge<TRANSPORT_EDGE_TYPES>*>* GetTargetEdges(const SESubstanceTransportVertex<TRANSPORT_VERTEX_TYPES>& v) const = 0;
};

template <SUBSTANCE_TRANSPORTER_TEMPLATE>
class CDM_DECL SESubstanceTransporter : public Loggable
{
public:
  SESubstanceTransporter(const FluxUnit& fUnit, const QuantityUnit& qUnit, const ExtensiveUnit& eUnit, const IntensiveUnit& iUnit, Logger* logger);
  virtual ~SESubstanceTransporter() {}

  void Transport(GraphType& graph, double timeStep_s);

protected:
  const FluxUnit      &m_FluxUnit;
  const QuantityUnit  &m_QuantityUnit;
  const ExtensiveUnit &m_ExtensiveUnit;
  const IntensiveUnit &m_IntensiveUnit;
};
