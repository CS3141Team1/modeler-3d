#include "FileIO.h"
#include <math.h>
#include "Types.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <vector>
#include <iostream>
#include <sstream>

/**
 * Class for loading and saving for 3d modeler
 *
 * @author Ryan Hamilton
 */

/**
 * Parses geometric vertex from line of OBJ file
 *
 * @param str - string containing geometric vertex
 *        refGeometricVertices - referance to a 2d vector for storing geometric vertex
 */
void FileIO::mParseGeometricVertex( std::string str, std::vector< std::vector< double > > &refGeometricVertices )
{
	double x, y, z;
	std::stringstream ss( str );
	char temp;
	char temp2;
	ss >> temp;
	ss >> temp2;
	ss >> x;
	ss >> y;
	ss >> z;

	refGeometricVertices[ refGeometricVertices.size( ) - 1 ].push_back( x );
	refGeometricVertices[ refGeometricVertices.size( ) - 1 ].push_back( y );
	refGeometricVertices[ refGeometricVertices.size( ) - 1 ].push_back( z );
}
 
 /**
 * Parses texture coordinates from line of OBJ file
 *
 * @param str - string containing texture coordinates
 *        refTextureCoordinates - referance to a 2d vector for storing texture coordinates
 */
 void FileIO::mParseTextureCoordinates( std::string str, std::vector< std::vector< double > > &refTextureCoordinates )
{
	double x, y;
	std::stringstream ss( str );
	char temp;
	ss >> temp;
	ss >> temp;
	ss >> x;
	ss >> y;

	refTextureCoordinates[ refTextureCoordinates.size( ) - 1 ].push_back( x );
	refTextureCoordinates[ refTextureCoordinates.size( ) - 1 ].push_back( y );
}

/**
 * Parses Normal Vertex from line of OBJ file
 *
 * @param str - string containing normal vertex
 *        refNormalVertices - referance to a 2d vector for storing Gnormal Vertices
 */
void FileIO::mParseNormalVertex( std::string str, std::vector< std::vector< double > > &refNormalVertices )
{
	double x, y, z;
	std::stringstream ss( str );
	char temp;
	char temp2;
	ss >> temp;
	ss >> temp2;
	ss >> x;
	ss >> y;
	ss >> z;

	refNormalVertices[ refNormalVertices.size( ) - 1 ].push_back( x );
	refNormalVertices[ refNormalVertices.size( ) - 1 ].push_back( y );
	refNormalVertices[ refNormalVertices.size( ) - 1 ].push_back( z );

}
 
/**
 * Parses face elements from line of OBJ file
 *
 * @param str - string containingface elements
 *        refFaceElements - referance to a 2d vector for storing face elements
 */
 void FileIO::mParseFaceElements( std::string str, std::vector< std::vector< std::vector< int > > > &refFaceElements )
{
	int v1, v2, v3;
	std::stringstream ss( str );
	char temp;
	ss >> temp;

	for( uint i = 0; i < 3; i++ )
	{
		ss >> v1;

		if( ss.peek( ) == '/' )
			ss >> temp;

		if( ss.peek( ) != '/' || ss.peek( ) != ' ' )
			ss >> v2;

		if( ss.peek( ) == '/' )
			ss >> temp;

		if( ss.peek( ) != '/' || ss.peek( ) != ' ' )
			ss >> v3;

		refFaceElements[ refFaceElements.size( ) - 1 ][ i ].push_back( v1 );
		refFaceElements[ refFaceElements.size( ) - 1 ][ i ].push_back( v2 );
		refFaceElements[ refFaceElements.size( ) - 1 ][ i ].push_back( v3 );
	}
}
 
/**
 * Save 3d model as a .obj
 *
 * @param p - A path
 */
void FileIO::SaveObj(boost::filesystem::path p)
{

}

/**
 * Save 3d model as a .obj
 *
 * @param p - A path
 *        newmGeometricVertices - A vector<vector<double>>
 *        newFaceElements - A vector<vector<vector<int>>>
 */
void FileIO::SaveObj(boost::filesystem::path p, std::vector<std::vector<double>> newGeometricVertices, std::vector<std::vector<int>> newFaceElements)
{

}

/**
 * Save 3d model as a .obj
 *
 * @param p - A path
 *        newmGeometricVertices - A vector<vector<double>>
 *		  newTextureVectices - A vector<vector<double>>
 *        newNormalVectices - A vector<vector<double>>
 *        newFaceElements - A vector<vector<vector<int>>>
 */
void FileIO::SaveObj(boost::filesystem::path p, std::vector<std::vector<double>> newmGeometricVertices,
		std::vector<std::vector<double>> newTextureVectices, std::vector<std::vector<double>> newNormalVectices, std::vector<std::vector<int>> mFaceElements)
		{

}

/*
 *
void FileIO::SaveObj(boost::filesystem::path p, std::vector<std::vector<double>> newmGeometricVertices,
		std::vector<std::vector<double>> newTextureVectices, std::vector<std::vector<double>> newNormalVectices,
		std::vector<std::vector<int>> newFaceElements){

}
*/

/**
 * Loads a .obj file and parses the file for the geometric vertices, texture coordinates, normal vertices, and face elements from the .obj file
 *
 * @param p - path of .obj file
 */
void FileIO::LoadObj(boost::filesystem::path p)
{
	boost::filesystem::ifstream File(p);
	std::string FileLine;
	
	//loops through entire .obj file
	while(std::getline(File, FileLine))
	{
		std::cout<<FileLine<<"\n";

		if(FileLine.size() > 2){
			
			//checks for Geometric vertex coordinates
			if(FileLine.at(0) == 'v' && FileLine.at(1) == ' ')
			{
				double X, Y, Z;
				std::stringstream S(FileLine);
				char Temp;
				S >> Temp;
				S >> X;
				S >> Y;
				S >> Z;

				std::vector<double> GeometricVector;
				GeometricVector.push_back(X);
				GeometricVector.push_back(Y);
				GeometricVector.push_back(Z);

				mGeometricVertices.push_back(GeometricVector);
			}

			//checks for texture coordinates
			if(FileLine.at(0) == 'v' && FileLine.at(1) == 't')
			{
				double X, Y;
				std::stringstream S(FileLine);
				char Temp;
				char Temp2;
				S >> Temp;
				S >> Temp2;
				S >> X;
				S >> Y;

				std::vector<double> TextureVector;

				TextureVector.push_back(X);
				TextureVector.push_back(Y);

				mTextureCoordinates.push_back(TextureVector);
			}

			//checks for normal vertex coordinates
			if(FileLine.at(0) == 'v' && FileLine.at(1) == 'n')
			{
				double X, Y, Z;
				std::stringstream S(FileLine);
				char Temp;
				char Temp2;
				S >> Temp;
				S >> Temp2;
				S >> X;
				S >> Y;
				S >> Z;

				std::vector<double> NormalVector;

				NormalVector.push_back(X);
				NormalVector.push_back(Y);
				NormalVector.push_back(Z);

				mNormalVertices.push_back(NormalVector);
			}

			//checks for face elements
			if(FileLine.at(0) == 'f' && FileLine.at(1) == ' ')
			{
				int V1, V2, V3;
				std::stringstream S(FileLine);
				char Temp;
				S >> Temp;
				std::vector<int> VerticeVector1;
				std::vector<std::vector<int>> FaceVector;
				
				//parses through first set of vertices
				S >> V1;
				VerticeVector1.push_back(V1);

				if(S.peek() == '/')
					S >> Temp;

				if(S.peek() != '/' || S.peek() != ' ')
				{
					S >> V2;
					VerticeVector1.push_back(V2);
				}
				else
					VerticeVector1.push_back(0);

				if(S.peek() == '/')
					S >> Temp;

				if(S.peek() != '/' || S.peek() != ' ')
				{
					S >> V3;
					VerticeVector1.push_back(V3);
				}
				else
					VerticeVector1.push_back(0);

				FaceVector.push_back(VerticeVector1);
				std::vector<int> VerticeVector2;

				// parses through second set of vertices
				S >> V1;
				VerticeVector2.push_back(V1);
				
				if(S.peek() == '/')
					S >> Temp;

				if(S.peek() != '/' || S.peek() != ' ')
				{
					S >> V2;
					VerticeVector2.push_back(V2);
				}
				else
					VerticeVector2.push_back(0);

				if(S.peek() == '/')
					S >> Temp;

				if(S.peek() != '/' || S.peek() != ' ')
				{
					S >> V3;
					VerticeVector2.push_back(V3);
				}
				else
					VerticeVector2.push_back(0);

				FaceVector.push_back(VerticeVector2);
				std::vector<int> VerticeVector3;

				//parses through the third set of vertices
				S >> V1;
				VerticeVector3.push_back(V1);
					
				if(S.peek() == '/')
					S >> Temp;

				if(S.peek() != '/' || S.peek() != ' ')
				{
					S >> V2;
					VerticeVector3.push_back(V2);
				}
				else
					VerticeVector3.push_back(0);
	
				if(S.peek() == '/')
					S >> Temp;
	
				if(S.peek() != '/' || S.peek() != ' ')
				{
					S >> V3;
					VerticeVector3.push_back(V3);
				}
				else
					VerticeVector3.push_back(0);

				FaceVector.push_back(VerticeVector3);
				mFaceElements.push_back(FaceVector);
			}
		}
	}
}

/**
 * Loads a .obj file and parses the file for the geometric vertices, texture coordinates, normal vertices, and face elements from the .obj file
 *
 * @param p - path of .obj file
		  refGeometricVertices
		  refTextureCoordinates
		  refNormalVertices
		  refFaceElements
 */
void FileIO::LoadObj2( boost::filesystem::path p, std::vector< std::vector< double > > &refGeometricVertices,
		std::vector< std::vector< double > > &refTextureCoordinates, std::vector< std::vector<double > > &refNormalVertices,
		std::vector< std::vector< std::vector< int > > > &refFaceElements )
{
		boost::filesystem::ifstream file( p );
		std::string fileLine;

		while( std::getline( file, fileLine ) )
		{
			if( fileLine.size( ) > 2 )
			{

				if( fileLine.at( 0 ) == 'v' && fileLine.at( 1 ) == ' ' )
				{
					std::vector< double > geometricVertex;
					refGeometricVertices.push_back( geometricVertex );
					mParseGeometricVertex( fileLine, refGeometricVertices );
				}

				if( fileLine.at( 0 ) == 'v' && fileLine.at( 1 ) == 't' )
				{
					std::vector< double > TextureCoordinate;
					refTextureCoordinates.push_back( TextureCoordinate );
					mParseTextureCoordinates( fileLine, refTextureCoordinates );
				}

				if( fileLine.at( 0 ) == 'v' && fileLine.at( 1 ) == 'n' )
				{
					std::vector< double > NormalVertex;
					refNormalVertices.push_back( NormalVertex );
					mParseNormalVertex( fileLine, refNormalVertices );
				}

				if( fileLine.at( 0 ) == 'f' && fileLine.at( 1 ) == ' ' )
				{
					std::vector< int > vertexSet1;
					std::vector< int > vertexSet2;
					std::vector< int > vertexSet3;

					std::vector< std::vector< int > > faceVertices;

					faceVertices.push_back( vertexSet1 );
					faceVertices.push_back( vertexSet2 );
					faceVertices.push_back(vertexSet3 );
					refFaceElements.push_back( faceVertices );

					mParseFaceElements( fileLine, refFaceElements );
				}
			}
		}
}

/**
 * Returns the geometric vertices of a .obj file
 *
 * @return mGeometricVertices - A vector<vector<double>>
 */
std::vector<std::vector<double>> FileIO::GetGeometricVertices()
{
	return mGeometricVertices;
}

/**
 * Sets mGeometricVertices to new values in order to be saved
 *
 * @param newGeometricVertices - A vector<vector<double>>
 */
void FileIO::SetGeometricVertices(std::vector<std::vector<double>> newGeometricVertices)
{

}

/**
 * Returns the texture vertices of a .obj file
 *
 * @return mTextureVertices - A vector<vector<double>>
 */
std::vector<std::vector<double>> FileIO::GetTextureCoordinates()
{
	return mTextureCoordinates;
}

/**
 * Sets mTextureVertices to new values in order to be saved
 *
 * @param newTextureVertices - A vector<vector<double>>
 */
void FileIO::SetTextureCoordinates(std::vector<std::vector<double>> newTextureCoordinates)
{

}

/**
 * Returns the normal vertices of a .obj file
 *
 * @return mNormalVertices - A vector<vector<double>>
 */
std::vector<std::vector<double>> FileIO::GetNormalVertices()
{
	return mNormalVertices;
}

/**
 * Sets mNormalVertices to new values in order to be saved
 *
 * @param newNormalVertices - A vector<vector<double>>
 */
void FileIO::SetNoramlVertices(std::vector<std::vector<double>> newNoramlVertices)
{

}
/**
 * Returns the face elements of a .obj file
 *
 * @return mFaceElements - A vector<vector<vector<int>>>
 */
std::vector<std::vector<std::vector<int>>> FileIO::GetFaceElements()
{
	return mFaceElements;
}

/**
 * Sets mFaceElements to new values in order to be saved
 *
 * @param newFaceElements - A vector<vector<vector<int>>>
 */
void FileIO::SetFaceElements(std::vector<std::vector<std::vector<int>>> newFaceElements)
{

}

