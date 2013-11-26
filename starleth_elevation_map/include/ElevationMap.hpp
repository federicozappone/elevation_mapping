/*
 * ElevationMap.hpp
 *
 *  Created on: Nov 12, 2013
 *      Author: Péter Fankhauser
 *	 Institute: ETH Zurich, Autonomous Systems Lab
 */

#pragma once

// StarlETH Navigation
#include <EigenConversions.hpp>

// Eigen
#include <Eigen/Core>
#include <Eigen/Geometry>

// PCL
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

// ROS
#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>

namespace starleth {

/*
 * Elevation map stored as planar grid holding elevation height and variance.
 */
class ElevationMap
{
 public:
  ElevationMap(ros::NodeHandle& nodeHandle);

  virtual ~ElevationMap();

  void pointCloudCallback(const sensor_msgs::PointCloud2& pointCloud);

  void timerCallback(const ros::TimerEvent& timerEvent);

  bool resize(double length, double width);

 private:
  bool readParameters();

  //! This makes the point cloud also dense (no NaN points).
  bool cleanPointCloud(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr pointCloud);

  bool transformPointCloud(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr pointCloud,
                           const std::string& targetFrame);

  bool addToElevationMap(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr pointCloud);

  bool publishElevationMap();

  bool broadcastElevationMapTransform();

  void setTimeOfLastUpdate(const ros::Time& timeOfLastUpdate);

  ros::NodeHandle& nodeHandle_;
  ros::Subscriber pointCloudSubscriber_;
  ros::Publisher elevationMapPublisher_;
  ros::Timer timer_;
  tf::TransformBroadcaster transformBroadcaster_;
  tf::TransformListener transformListener_;

  ros::Time timeOfLastUpdate_;

  //! Elevation height data.
  Eigen::MatrixXd elevationData_;

  //! Variance data of the cells in elevationData_.
  Eigen::MatrixXd varianceData_;

  //! Map length (x-direction) [m].
  double length_;

  //! Map width (y-direction) [m].
  double width_;

  //! Map resolution in xy plane [m/cell].
  double resolution_;

  //! Origin of the map.
  Eigen::Affine3d elevationMapToParentTransform_;
  std::string parentFrameId_;
  std::string elevationMapFrameId_;

  std::string pointCloudTopic;

  double sensorCutoffDepth_;


//  //! Values are in the range [0, 32768]. 0 belongs to max negative elevation,
//  //! 32767 to max positive elevation, elevationZeroLevel_ corresponds to zero level,
//  //! -elevationZeroLevel_ to unknown elevation.
//
//  //! Map resolution in height [m/cell].
//  double resolutionHeight_;
//
//  //! Minimal observed height [m].
//  double minElevation_;
//
//  //! Maximal observed height [m].
//  double maxElevation_;
//
//  //! Value of the zero height (for example 16384).
//  int elevationZeroLevel_;

};

} /* namespace starleth */
