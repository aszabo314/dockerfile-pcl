docker container with ubuntu, checkout and build PCL. includes .cpp file running segmentation example (adjust local paths).

> [+] Building 2915.2s (27/27) FINISHED

```
docker build -t pclubuntu .
docker run -it -v C:\proj\docker-pcl\share:/things pclubuntu
```

run in /things:

```
cmake .
make && ./segmentation imst_out.pcd
```