Function Get-AbsolutePath {

    <#
    
        .SYNOPSIS
        Get the absolute path.
        .DESCRIPTION
        Provides a simple way for transforming any path, relative or not,
        into an absolute path. If no path is specified, the current working
        directory is returned.
        .PARAMETER relativePath
        The path to be transformed into absolute. If not specified, then the current working directory is used.
    
    #>

    [CmdletBinding()]
    Param(
        [parameter(
            Mandatory=$false,
            ValueFromPipeline=$true
        )]
        [String]$relativePath=".\"
    )

    if (Test-Path -Path $relativePath) {
        return (Get-Item -Path $relativePath).FullName -replace "\\$", ""
    } else {
        Write-Error -Message "'$relativePath' is not a valid path" -ErrorId 1 -ErrorAction Stop
    }

}

Function BinaryQuestion {

    <#
    
        .SYNOPSIS
        Yes or no question.
        .DESCRIPTION
        Provides an easy way for capturing a user-desired action
        based on text input. If the user types 'y[es]', boolean
        TRUE is returned. Any other response is considered FALSE.
        .PARAMETER message
        The message/question that will be output to the host.
    
    #>

    [CmdletBinding()]
    Param(
        [parameter(
            Mandatory=$true,
            ValueFromPipeline=$true
        )]
        [String]$message
    )

    return ((Read-Host $message) -match "^y(es)?$")

}