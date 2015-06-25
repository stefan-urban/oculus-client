#ifndef ERRORS_HPP
#define ERRORS_HPP

#include "Common.h"


namespace oglplus
{
    inline void print_std_error_common(
        std::exception& error,
        std::ostream& errstr
    )
    {
        errstr	<< "Message: '"
            << error.what()
            << "'"
            << std::endl;
    }

    inline void print_error_common(
        Error& error,
        std::ostream& errstr
    )
    {
        if(error.SourceFile())
        {
            errstr	<< "Source file: '"
                << error.SourceFile()
                << "'"
                << std::endl;
        }

        if(error.SourceLine())
        {
            errstr	<< "Source line: "
                << error.SourceLine()
                << std::endl;
        }

        if(error.SourceFunc())
        {
            errstr	<< "Source function: '"
                << error.SourceFunc()
                << "'"
                << std::endl;
        }
        print_std_error_common(error, errstr);
        if(error.GLFunc())
        {
            errstr	<< "GL function: '";

            if(error.GLLib())
            {
                errstr << error.GLLib();
            }

            errstr	<< error.GLFunc()
                << "'"
                << std::endl;
        }


        if(error.EnumParam() || error.EnumParamName())
        {
            errstr	<< "GL constant: ";
            if(error.EnumParamName())
            {
                errstr	<< "'"
                    << error.EnumParamName()
                    << "'";
            }
            else
            {
                errstr	<< "(0x"
                    << std::hex
                    << error.EnumParam()
                    << ")";
            }
            errstr	<< std::endl;
        }

        if(error.BindTarget() || error.TargetName())
        {
            errstr	<< "Binding point: ";
            if(error.TargetName())
            {
                errstr	<< "'"
                    << error.TargetName()
                    << "'";
            }
            else
            {
                errstr	<< "(0x"
                    << std::hex
                    << error.BindTarget()
                    << ")";
            }
            errstr	<< std::endl;
        }

        if(error.ObjectTypeName() || error.ObjectType())
        {
            errstr	<< "Object type: ";
            if(error.ObjectTypeName())
            {
                errstr	<< "'"
                    << error.ObjectTypeName()
                    << "'";
            }
            else
            {
                errstr	<< "(0x"
                    << std::hex
                    << error.ObjectType()
                    << ")";
            }
            errstr	<< std::endl;
        }

        if((!error.ObjectDesc().empty()) || (error.ObjectName() >= 0))
        {
            errstr	<< "Object: ";
            if(!error.ObjectDesc().empty())
            {
                errstr	<< "'"
                    << error.ObjectDesc()
                    << "'";
            }
            else
            {
                errstr	<< "("
                    << error.ObjectName()
                    << ")";
            }
            errstr	<< std::endl;
        }

        if(error.SubjectTypeName() || error.SubjectType())
        {
            errstr	<< "Subject type: ";
            if(error.SubjectTypeName())
            {
                errstr	<< "'"
                    << error.SubjectTypeName()
                    << "'";
            }
            else
            {
                errstr	<< "(0x"
                    << std::hex
                    << error.SubjectType()
                    << ")";
            }
            errstr	<< std::endl;
        }

        if((!error.SubjectDesc().empty()) || (error.SubjectName() >= 0))
        {
            errstr	<< "Subject: ";
            if(!error.SubjectDesc().empty())
            {
                errstr	<< "'"
                    << error.SubjectDesc()
                    << "'";
            }
            else
            {
                errstr	<< "("
                    << error.SubjectName()
                    << ")";
            }
            errstr	<< std::endl;
        }

        if(error.Identifier())
        {
            errstr	<< "Identifier: '"
                << error.Identifier()
                << "'"
                << std::endl;
        }

        if(error.Index() >= 0)
        {
            errstr	<< "Index: ("
                << error.Index()
                << ")"
                << std::endl;
        }

        if(error.Value() != 0)
        {
            errstr	<< "Value: ("
                << error.Value()
                << ")"
                << std::endl;
        }

        if(error.Limit() != 0)
        {
            errstr	<< "Limit: ("
                << error.Limit()
                << ")"
                << std::endl;
        }

        if(!error.Log().empty())
        {
            errstr	<< "Log:"
                << std::endl
                << error.Log()
                << std::endl;
        }
    }

}


#endif // ERRORS_HPP

