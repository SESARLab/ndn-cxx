/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2018 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#ifndef NDN_MGMT_NFD_FACE_STATUS_HPP
#define NDN_MGMT_NFD_FACE_STATUS_HPP

#include "ndn-cxx/mgmt/nfd/face-traits.hpp"
#include "ndn-cxx/util/time.hpp"

namespace ndn {
namespace nfd {

/**
 * \ingroup management
 * \brief represents an item in NFD Face dataset
 * \sa https://redmine.named-data.net/projects/nfd/wiki/FaceMgmt#Face-Dataset
 */
class FaceStatus : public FaceTraits<FaceStatus>
{
public:
  FaceStatus();

  explicit
  FaceStatus(const Block& block);

  /** \brief prepend FaceStatus to the encoder
   */
  template<encoding::Tag TAG>
  size_t
  wireEncode(EncodingImpl<TAG>& encoder) const;

  /** \brief encode FaceStatus
   */
  const Block&
  wireEncode() const;

  /** \brief decode FaceStatus
   */
  void
  wireDecode(const Block& wire);

public: // getters & setters
  bool
  hasExpirationPeriod() const
  {
    return !!m_expirationPeriod;
  }

  time::milliseconds
  getExpirationPeriod() const
  {
    BOOST_ASSERT(hasExpirationPeriod());
    return *m_expirationPeriod;
  }

  FaceStatus&
  setExpirationPeriod(time::milliseconds expirationPeriod);

  FaceStatus&
  unsetExpirationPeriod();

  bool
  hasBaseCongestionMarkingInterval() const
  {
    return !!m_baseCongestionMarkingInterval;
  }

  time::nanoseconds
  getBaseCongestionMarkingInterval() const
  {
    BOOST_ASSERT(hasBaseCongestionMarkingInterval());
    return *m_baseCongestionMarkingInterval;
  }

  FaceStatus&
  setBaseCongestionMarkingInterval(time::nanoseconds interval);

  FaceStatus&
  unsetBaseCongestionMarkingInterval();

  bool
  hasDefaultCongestionThreshold() const
  {
    return !!m_defaultCongestionThreshold;
  }

  /** \brief get default congestion threshold (measured in bytes)
   */
  uint64_t
  getDefaultCongestionThreshold() const
  {
    BOOST_ASSERT(hasDefaultCongestionThreshold());
    return *m_defaultCongestionThreshold;
  }

  /** \brief set default congestion threshold (measured in bytes)
   */
  FaceStatus&
  setDefaultCongestionThreshold(uint64_t threshold);

  FaceStatus&
  unsetDefaultCongestionThreshold();

  bool
  hasMtu() const
  {
    return !!m_mtu;
  }

  /** \brief get MTU (measured in bytes)
   *
   *  This value is capped at MAX_NDN_PACKET_SIZE, even if the MTU of the face is unlimited.
   */
  uint64_t
  getMtu() const
  {
    BOOST_ASSERT(hasMtu());
    return *m_mtu;
  }

  /** \brief set MTU (measured in bytes)
   *
   *  This value is capped at MAX_NDN_PACKET_SIZE, even if the MTU of the face is unlimited.
   */
  FaceStatus&
  setMtu(uint64_t mtu);

  FaceStatus&
  unsetMtu();

  uint64_t
  getNInInterests() const
  {
    return m_nInInterests;
  }

  FaceStatus&
  setNInInterests(uint64_t nInInterests);

  uint64_t
  getNInData() const
  {
    return m_nInData;
  }

  FaceStatus&
  setNInData(uint64_t nInData);

  uint64_t
  getNInNacks() const
  {
    return m_nInNacks;
  }

  FaceStatus&
  setNInNacks(uint64_t nInNacks);

  uint64_t
  getNOutInterests() const
  {
    return m_nOutInterests;
  }

  FaceStatus&
  setNOutInterests(uint64_t nOutInterests);

  uint64_t
  getNOutData() const
  {
    return m_nOutData;
  }

  FaceStatus&
  setNOutData(uint64_t nOutData);

  uint64_t
  getNOutNacks() const
  {
    return m_nOutNacks;
  }

  FaceStatus&
  setNOutNacks(uint64_t nOutNacks);

  uint64_t
  getNInBytes() const
  {
    return m_nInBytes;
  }

  FaceStatus&
  setNInBytes(uint64_t nInBytes);

  uint64_t
  getNOutBytes() const
  {
    return m_nOutBytes;
  }

  FaceStatus&
  setNOutBytes(uint64_t nOutBytes);

  uint64_t
  get_interest_packet_size_min() const
  {
    return m_interest_packet_size_min;
  }

  FaceStatus&
  set_interest_packet_size_min(uint64_t interest_packet_size_min);

  uint64_t
  get_interest_packet_size_max() const
  {
    return m_interest_packet_size_max;
  }

  FaceStatus&
  set_interest_packet_size_max(uint64_t interest_packet_size_max);

  float
  get_interest_packet_size_avg() const
  {
    return m_interest_packet_size_avg;
  }

  FaceStatus&
  set_interest_packet_size_avg(float interest_packet_size_avg);

  float
  get_interest_packet_size_std_dev() const
  {
    return m_interest_packet_size_std_dev;
  }

  FaceStatus&
  set_interest_packet_size_std_dev(float interest_packet_size_std_dev);

  uint64_t
  get_data_packet_size_min() const
  {
    return m_data_packet_size_min;
  }

  FaceStatus&
  set_data_packet_size_min(uint64_t data_packet_size_min);

  uint64_t
  get_data_packet_size_max() const
  {
    return m_data_packet_size_max;
  }

  FaceStatus&
  set_data_packet_size_max(uint64_t data_packet_size_max);

  float
  get_data_packet_size_avg() const
  {
    return m_data_packet_size_avg;
  }

  FaceStatus&
  set_data_packet_size_avg(float data_packet_size_avg);

  float
  get_data_packet_size_std_dev() const
  {
    return m_data_packet_size_std_dev;
  }

  FaceStatus&
  set_data_packet_size_std_dev(float data_packet_size_std_dev);

  uint64_t
  get_interest_packet_components_min() const
  {
    return m_interest_packet_components_min;
  }

  FaceStatus&
  set_interest_packet_components_min(uint64_t interest_packet_components_min);

  uint64_t
  get_interest_packet_components_max() const
  {
    return m_interest_packet_components_max;
  }

  FaceStatus&
  set_interest_packet_components_max(uint64_t interest_packet_components_max);

  float
  get_interest_packet_components_avg() const
  {
    return m_interest_packet_components_avg;
  }

  FaceStatus&
  set_interest_packet_components_avg(float interest_packet_components_avg);

  float
  get_interest_packet_components_std_dev() const
  {
    return m_interest_packet_components_std_dev;
  }

  FaceStatus&
  set_interest_packet_components_std_dev(float interest_packet_components_std_dev);

  uint64_t
  get_data_packet_components_min() const
  {
    return m_data_packet_components_min;
  }

  FaceStatus&
  set_data_packet_components_min(uint64_t data_packet_components_min);

  uint64_t
  get_data_packet_components_max() const
  {
    return m_data_packet_components_max;
  }

  FaceStatus&
  set_data_packet_components_max(uint64_t data_packet_components_max);

  float
  get_data_packet_components_avg() const
  {
    return m_data_packet_components_avg;
  }

  FaceStatus&
  set_data_packet_components_avg(float data_packet_components_avg);

  float
  get_data_packet_components_std_dev() const
  {
    return m_data_packet_components_std_dev;
  }

  FaceStatus&
  set_data_packet_components_std_dev(float data_packet_components_std_dev);

private:
  optional<time::milliseconds> m_expirationPeriod;
  optional<time::nanoseconds> m_baseCongestionMarkingInterval;
  optional<uint64_t> m_defaultCongestionThreshold;
  optional<uint64_t> m_mtu;
  uint64_t m_nInInterests;
  uint64_t m_nInData;
  uint64_t m_nInNacks;
  uint64_t m_nOutInterests;
  uint64_t m_nOutData;
  uint64_t m_nOutNacks;
  uint64_t m_nInBytes;
  uint64_t m_nOutBytes;

  uint64_t m_interest_packet_size_min;
  uint64_t m_interest_packet_size_max;
  float m_interest_packet_size_avg;
  float m_interest_packet_size_std_dev;
  uint64_t m_data_packet_size_min;
  uint64_t m_data_packet_size_max;
  float m_data_packet_size_avg;
  float m_data_packet_size_std_dev;
  uint64_t m_interest_packet_components_min;
  uint64_t m_interest_packet_components_max;
  float m_interest_packet_components_avg;
  float m_interest_packet_components_std_dev;
  uint64_t m_data_packet_components_min;
  uint64_t m_data_packet_components_max;
  float m_data_packet_components_avg;
  float m_data_packet_components_std_dev;
};

NDN_CXX_DECLARE_WIRE_ENCODE_INSTANTIATIONS(FaceStatus);

bool
operator==(const FaceStatus& a, const FaceStatus& b);

inline bool
operator!=(const FaceStatus& a, const FaceStatus& b)
{
  return !(a == b);
}

std::ostream&
operator<<(std::ostream& os, const FaceStatus& status);

} // namespace nfd
} // namespace ndn

#endif // NDN_MGMT_NFD_FACE_STATUS_HPP
