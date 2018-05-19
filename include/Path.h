/*
    Copyright (c) 2015, Christopher Nitta
    All rights reserved.

    All source material (source code, images, sounds, etc.) have been provided to
    University of California, Davis students of course ECS 160 for educational
    purposes. It may not be distributed beyond those enrolled in the course without
    prior permission from the copyright holder.

    All sound files, sound fonts, midi files, and images that have been included 
    that were extracted from original Warcraft II by Blizzard Entertainment 
    were found freely available via internet sources and have been labeld as 
    abandonware. They have been included in this distribution for educational 
    purposes only and this copyright notice does not attempt to claim any 
    ownership of this material.
*/
#ifndef PATH_H
#define PATH_H

#include <string>
#include <vector>

class CPath{
    protected:
        std::vector< std::string > DDecomposedPath;
        bool DIsRelative;
        bool DIsValid;
        
        static bool DecomposePath(std::vector< std::string > &decomp, const std::string &path);
        static const char DDelimiter;
        static const std::string DDelimiterString;
        
    public:
        CPath();
        CPath(const CPath &path);
        CPath(const std::string &path);
        
        CPath &operator=(const CPath &path);
        
        CPath Containing() const;
        
        bool IsRelative() const{
            return DIsRelative;  
        };
        
        bool IsAbsolute() const{
            return !DIsRelative;  
        };
        
        bool IsValid() const{
            return DIsValid;  
        };
        
        int ComponentCount() const{
            return DDecomposedPath.size();
        };
        
        std::string Component(int index) const{
            if((0 > index)||(index >= DDecomposedPath.size())){
                return "";   
            }
            return DDecomposedPath[index];  
        };
        
        std::string ToString() const;
        operator std::string() const;
        
        CPath Simplify(const CPath &destpath) const;
        CPath Relative(const CPath &destpath) const;
        
        static CPath SimplifyPath(const CPath &srcpath, const CPath &destpath);
        static CPath RelativePath(const CPath &srcpath, const CPath &destpath);
        static CPath CurrentPath();
        static CPath CurrentPath(const CPath &path);
        
};

#endif
