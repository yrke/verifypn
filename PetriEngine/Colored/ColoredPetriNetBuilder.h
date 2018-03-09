/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ColoredPetriNetBuilder.h
 * Author: Klostergaard
 *
 * Created on 17. februar 2018, 16:25
 */

#ifndef COLOREDPETRINETBUILDER_H
#define COLOREDPETRINETBUILDER_H

#include <vector>
#include <unordered_map>

#include "ColoredNetStructures.h"
#include "../AbstractPetriNetBuilder.h"
#include "../PetriNetBuilder.h"

namespace PetriEngine {
    class ColoredPetriNetBuilder : public AbstractPetriNetBuilder {
    public:
        typedef std::unordered_map<std::string, Colored::ColorType*> ColorTypeMap;
        typedef std::unordered_map<std::string, std::unordered_map<uint32_t , std::string>> PTPlaceMap;
        typedef std::unordered_map<std::string, std::vector<std::string>> PTTransitionMap;
        
    public:
        ColoredPetriNetBuilder();
        ColoredPetriNetBuilder(const ColoredPetriNetBuilder& orig);
        virtual ~ColoredPetriNetBuilder();
        
        void addPlace(const std::string& name,
                int tokens,
                double x = 0,
                double y = 0) override ;
        void addPlace(const std::string& name,
                Colored::ColorType* type,
                Colored::Multiset tokens,
                double x = 0,
                double y = 0) override;
        void addTransition(const std::string& name,
                double x = 0,
                double y = 0) override;
        void addTransition(const std::string& name,
                Colored::GuardExpression* guard,
                double x = 0,
                double y = 0) override;
        void addInputArc(const std::string& place,
                const std::string& transition,
                bool inhibitor,
                int) override;
        void addInputArc(const std::string& place,
                const std::string& transition,
                Colored::ArcExpression* expr) override;
        void addOutputArc(const std::string& transition,
                const std::string& place,
                int weight = 1) override;
        void addOutputArc(const std::string& transition,
                const std::string& place,
                Colored::ArcExpression* expr) override;
        void addColorType(const std::string& id,
                Colored::ColorType* type) override;


        void sort() override;

        double getUnfoldTime() const {
            return _time;
        }

        uint32_t getPlaceCount() const {
            return _places.size();
        }

        uint32_t getTransitionCount() const {
            return _transitions.size();
        }

        uint32_t getArcCount() const {
            return _arcs.size();
        }

        uint32_t getUnfoldedPlaceCount() const {
            //return _nptplaces;
            return _ptBuilder.numberOfPlaces();
        }

        uint32_t getUnfoldedTransitionCount() const {
            return _ptBuilder.numberOfTransitions();
        }

        uint32_t getUnfoldedArcCount() const {
            return _nptarcs;
        }

        bool isUnfolded() const {
            return _unfolded;
        }

        const PTPlaceMap& getUnfoldedPlaceNames() const {
            return _ptplacenames;
        }

        const PTTransitionMap& getUnfoldedTransitionNames() const {
            return _pttransitionnames;
        }
        
        PetriNetBuilder& unfold();
    private:
        std::unordered_map<std::string,uint32_t> _placenames;
        std::unordered_map<std::string,uint32_t> _transitionnames;
        PTPlaceMap _ptplacenames;
        PTTransitionMap _pttransitionnames;
        uint32_t _nptplaces;
        uint32_t _npttransitions;
        uint32_t _nptarcs;
        
        std::vector<Colored::Place> _places;
        std::vector<Colored::Transition> _transitions;
        std::vector<Colored::Arc> _arcs;
        ColorTypeMap _colors;
        PetriNetBuilder _ptBuilder;
        bool _unfolded;

        double _time;
        
        void addArc(const std::string& place,
                const std::string& transition,
                Colored::ArcExpression* expr,
                bool input);
        
        void unfoldPlace(Colored::Place& place);
        void unfoldTransition(Colored::Transition& transition);
        void unfoldArc(Colored::Arc& arc);
    };
    
    class BindingGenerator {
    public:
        class Iterator {
        private:
            BindingGenerator* _generator;
            
        public:
            Iterator(BindingGenerator* generator);
            
            bool operator==(Iterator& other);
            bool operator!=(Iterator& other);
            Iterator& operator++();
            std::vector<Colored::Binding> operator++(int);
            std::vector<Colored::Binding>& operator*();
        };
    private:
        Colored::GuardExpression* _expr;
        std::vector<Colored::Binding> _bindings;
        ColoredPetriNetBuilder::ColorTypeMap& _colorTypes;
        
        bool eval();
        
    public:
        BindingGenerator(Colored::Transition& transition,
                const std::vector<Colored::Arc>& arcs,
                ColoredPetriNetBuilder::ColorTypeMap& colorTypes);
        
        std::vector<Colored::Binding>& nextBinding();
        std::vector<Colored::Binding>& currentBinding();
        bool isInitial() const;
        Iterator begin();
        Iterator end();
    };
}

#endif /* COLOREDPETRINETBUILDER_H */

