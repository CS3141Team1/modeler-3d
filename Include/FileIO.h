#pragma once

#include <boost/filesystem.hpp>
#include <vector>
#include <iostream>

/**
 * Class header file for loading and saving for 3d modeler
 *
 * @author Ryan Hamilton
 */
class FileIO{
	private:
		std::vector<std::vector<double>> mGeometricVertices;
		std::vector<std::vector<double>> mTextureVertices;
		std::vector<std::vector<double>> mNormalVertices;
		std::vector<std::vector<std::vector<int>>> mFaceElements;
		
	public:
	
		// load method for .obj files
		void LoadObj(boost::filesystem::path p);
		
		// save method for .obj files
		void SaveObj(std::string filename);

		// Get and Set methods for geometric vertices
		std::vector<std::vector<double>> getGeometricVertices();
		void setGeometricVertices(std::vector<std::vector<double>> newGeometricVertices);
		std::vector<std::vector<double>> getTextureVertices();
		void setTextureVertices(std::vector<std::vector<double>> newTextureVertices);
		std::vector<std::vector<double>> getNormalVertices();
		void setNoramlVertices(std::vector<std::vector<double>> newNoramlVertices);

		std::vector<std::vector<std::vector<int>>> getFaceElements();
		void setFaceElements(std::vector<std::vector<std::vector<int>>> newFaceElements);
		void Test();
};
