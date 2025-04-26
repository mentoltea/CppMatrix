Name:           matrix
Version:        1.0.0
Release:        1%{?dist}
Summary:        A simple matrix library for C++
BuildArch:      noarch

License:        MIT
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc-c++

%description
A RPM package for TSPP

%prep
%setup -q

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_includedir}
cp %{name}.hpp $RPM_BUILD_ROOT/%{_includedir}

%clean
rm -rf $RPM_BUILD_ROOT

%files
%{_includedir}/%{name}.hpp

%changelog
* Sat Apr 26 2025 Your Name <your.email@example.com> - ${VERSION}-${RELEASE}
- Initial package