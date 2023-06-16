#pragma once

#include <complexities/complexity_analyzer.h>
#include "libds/adt/table.h"
#include <random>
#include <vector>

namespace ds::utils
{
    /**
     * @brief Common base for list analyzers.
     */
    template<class Table>
    class TableAnalyzer : public ComplexityAnalyzer<Table>
    {
    protected:
        explicit TableAnalyzer(const std::string& name);

    protected:
        void beforeOperation(Table& structure) override;
        size_t getRandomIndex() const;
        int getRandomData() const;

    private:
        void insertNElements(Table& list, size_t n);

    private:
        std::default_random_engine rngData_;
        std::default_random_engine rngIndex_;
        size_t index_;
        int data_;
    };

    /**
     * @brief Analyzes complexity of the insert operation.
     */
    template<class Table>
    class TableInsertAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
        int index;
    };

    template<class Table>
    class TableFindAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableFindAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
        int index;
    };

    /**
     * @brief Container for all list analyzers.
     */
    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer() :
            CompositeAnalyzer("Tables")
        {
            this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::HashTable<int,int>>>("HashTable-insert"));
            this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::HashTable<int, int>>>("HashTable-find"));
        }
    };

    template<class Table>
    TableAnalyzer<Table>::TableAnalyzer(const std::string& name) :
        ComplexityAnalyzer<Table>(name, [this](Table& table, size_t n)
            {
                this->insertNElements(table, n);
            }),
        rngData_(144),
                rngIndex_(144),
                index_(0),
                data_(0)
    {
    }

            template<class Table>
            void TableAnalyzer<Table>::beforeOperation(Table& structure)
            {
                std::uniform_int_distribution<size_t> indexDist(0, structure.size() - 1);
                index_ = indexDist(this->rngIndex_);
                data_ = rngData_();
            }

            template<class Table>
            size_t TableAnalyzer<Table>::getRandomIndex() const
            {
                return index_;
            }

            template<class Table>
            int TableAnalyzer<Table>::getRandomData() const
            {
                return data_;
            }

            template<class Table>
            void TableAnalyzer<Table>::insertNElements(Table& table, size_t n)
            {
                for (size_t i = 0; i < n; ++i)
                {
                    //table.insert(i, i);
                }
            }

            template <class Table>
            TableInsertAnalyzer<Table>::TableInsertAnalyzer(const std::string& name) :
                TableAnalyzer<Table>(name)
            {
                index = 0;
            }

            template <class Table>
            void TableInsertAnalyzer<Table>::executeOperation(Table& structure)
            {
                auto data = this->getRandomData();
                index++;
                structure.insert(index, data);
            }

            template <class Table>
            TableFindAnalyzer<Table>::TableFindAnalyzer(const std::string& name) :
                TableAnalyzer<Table>(name)
            {
                index = 0;
            }

            template <class Table>
            void TableFindAnalyzer<Table>::executeOperation(Table& structure)
            {
                index++;
                int* ptr = &index;
                int*& ind = ptr;
                structure.tryFind(index,ind);
                
            }
            
}