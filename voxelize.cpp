#include <boost/program_options.hpp>
#include <H5Cpp.h>
#include <iostream>
#include "off_loader.h"
#include "voxelization.h"

namespace po = boost::program_options;

template<typename T>
void save_hdf5(const std::string& path,
               const std::vector<T>& data,
               const VolumeDims& dims)
{
    hsize_t shape[3] = {static_cast<hsize_t>(dims.D),
                        static_cast<hsize_t>(dims.H),
                        static_cast<hsize_t>(dims.W)}; // D x H x W

    H5::H5File file(path, H5F_ACC_TRUNC);
    H5::DataSpace space(3, shape);

    H5::PredType dtype = std::is_same<T,uint8_t>::value ?
                         H5::PredType::NATIVE_UINT8 :
                         H5::PredType::NATIVE_FLOAT;

    H5::DataSet ds = file.createDataSet("volume", dtype, space);
    ds.write(data.data(), dtype);
}

int main(int argc, char** argv)
{
    std::string mode, input, output;
    int H=32, W=32, D=32;
    bool use_centers=false;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("mode", po::value<std::string>(&mode)->default_value("occ"), "occ or sdf")
        ("input", po::value<std::string>(&input)->required(), "input OFF file")
        ("output", po::value<std::string>(&output)->required(), "output H5 file")
        ("height", po::value<int>(&H)->default_value(32), "grid height")
        ("width", po::value<int>(&W)->default_value(32), "grid width")
        ("depth", po::value<int>(&D)->default_value(32), "grid depth")
        ("center", po::bool_switch(&use_centers), "use voxel centres for SDF")
    ;

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 0;
        }
        po::notify(vm);
    } catch (po::error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return 1;
    }

    // Load mesh
    std::vector<Triangle> tris;
    if (!load_off(input, tris)) return 1;

    VolumeDims dims{H, W, D};

    if (mode == "occ") {
        std::vector<uint8_t> vol;
        voxelize_occupancy(tris, dims, vol);
        save_hdf5(output, vol, dims);
    } else if (mode == "sdf") {
        std::vector<float> sdf;
        voxelize_sdf(tris, dims, use_centers, sdf);
        save_hdf5(output, sdf, dims);
    } else {
        std::cerr << "Invalid mode '" << mode << "'. Use 'occ' or 'sdf'." << std::endl;
        return 1;
    }
    std::cout << "Saved " << output << std::endl;
    return 0;
}
