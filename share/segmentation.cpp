
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/search/search.h>
#include <pcl/search/kdtree.h>
#include <pcl/features/normal_3d.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/segmentation/region_growing.h>
#include <pcl/search/impl/kdtree.hpp>
#include <pcl/kdtree/impl/kdtree_flann.hpp>

int
main (int argc, char** argv)
{
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  if ( pcl::io::loadPCDFile <pcl::PointXYZ> (argv[1], *cloud) == -1)
  {
    std::cout << "Cloud reading failed." << std::endl;
    return (-1);
  }
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "read points = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;


  std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
  pcl::search::Search<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
  pcl::PointCloud <pcl::Normal>::Ptr normals (new pcl::PointCloud <pcl::Normal>);
  pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normal_estimator;
  normal_estimator.setSearchMethod (tree);
  normal_estimator.setInputCloud (cloud);
  normal_estimator.setKSearch (50);
  normal_estimator.compute (*normals);

  pcl::IndicesPtr indices (new std::vector <int>);
  pcl::PassThrough<pcl::PointXYZ> pass;
  pass.setInputCloud (cloud);
  pass.setFilterFieldName ("z");
  pass.setFilterLimits (0.0, 1.0);
  pass.filter (*indices);

  // std::cout << "loopy" << std::endl;
  // for(int pi = 0; pi<100; pi++) {
  //   std::cout << (*cloud).points[pi].x << " " << (*cloud).points[pi].y << " " << (*cloud).points[pi].z << " " << "ci" << std::endl;;
  // }
  // std::vector<pcl::PointXYZ> data = (*cloud).points;
  // std::cout << "pts " << data.size () << std::endl;

  pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> reg;
  reg.setMinClusterSize (50);
  reg.setMaxClusterSize (1000000);
  reg.setSearchMethod (tree);
  reg.setNumberOfNeighbours (30);
  reg.setInputCloud (cloud);
  //reg.setIndices (indices);
  reg.setInputNormals (normals);
  reg.setSmoothnessThreshold (3.0 / 180.0 * M_PI);
  reg.setCurvatureThreshold (1.0);

  std::vector <pcl::PointIndices> clusters;
  reg.extract (clusters);

  std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
  std::cout << "region growing = " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2).count() << "[µs]" << std::endl;

  std::cout << "number of clusters: " << clusters.size () << std::endl;

  const auto p1 = std::chrono::system_clock::now();
  const int time = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
  const auto filename = "/things/out_"+ std::to_string(time) +".pts";
  std::ofstream out;
  out.open(filename, std::ios_base::app);
  //std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  //std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

  //std::string word;
  //std::cin >> word;           //input from the file in.txt
  //std::cout << word << "  ";  //output to the file out.txt


  std::cout << "here comes out loop:" << std::endl;
  for(int ci=0; ci<clusters.size(); ci++) {
      std::cout << "cluster=" << ci << std::endl;
      for(int pi=0; pi<clusters[ci].indices.size();pi++) {
        const auto pidx = clusters[ci].indices[pi];
        out << (*cloud).points[pidx].x << " " << (*cloud).points[pidx].y << " " << (*cloud).points[pidx].z << " " << ci << std::endl;;
        //std::cout << clusters[ci].indices[pi] << std::endl;
      }
  }
  std::cout << "File write finished." << std::endl;

//   std::cout << "First cluster has " << clusters[0].indices.size () << " points." << std::endl;
//   std::cout << "These are the indices of the points of the initial" <<
//     std::endl << "cloud that belong to the first cluster:" << std::endl;
//   int counter = 0;
//   while (counter < clusters[0].indices.size ())
//   {
//     std::cout << clusters[0].indices[counter] << ", ";
//     counter++;
//     if (counter % 10 == 0)
//       std::cout << std::endl;
//   }
//   std::cout << std::endl;

//   pcl::PointCloud <pcl::PointXYZRGB>::Ptr colored_cloud = reg.getColoredCloud ();
//   pcl::visualization::CloudViewer viewer ("Cluster viewer");
//   viewer.showCloud(colored_cloud);
//   while (!viewer.wasStopped ())
//   {
//   }

  return (0);
}
